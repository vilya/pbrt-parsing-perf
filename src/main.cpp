#include <minipbrt.h>
#include <pbrtParser/Scene.h>
#include <vh_cmdline.h>
#include <vh_time.h>

#include <atomic>
#include <cstdio>
#include <string>
#include <thread>

//
// Constants
//

static const char* kTitle = "PBRT Parsing Performance Comparison";
static const int kMajorVersion = 0;
static const int kMinorVersion = 0;

enum CmdLineOption {
  eHelp,
  eVersion,
  eNoMiniPBRT,
  eNoThreaded,
  eNoPBRTParser,
  eNoPrewarm,
  eCSV,
  eQuiet,
  eOutFile,
};

static const vh::CommandLineOption options[] = {
  { eHelp,              'h',  "help",          nullptr, nullptr, "Print this help message and exit."                          },
  { eVersion,           'v',  "version",       nullptr, nullptr, "Print the application version and exit."                    },
  { eNoMiniPBRT,        '\0', "no-minipbrt",   nullptr, nullptr, "Disable the \"minpbrt\" parser."                            },
  { eNoThreaded,        '\0', "no-threaded",   nullptr, nullptr, "Disable the \"threaded\" parser (minipbrt, with multithreaded PLY mesh loading)." },
  { eNoPBRTParser,      '\0', "no-pbrtparser", nullptr, nullptr, "Disable the \"pbrt-parser\" parser."                        },
  { eNoPrewarm,         '\0', "no-prewarm",    nullptr, nullptr, "Don't pre-warm the disk cache before parsing (useful for very large scenes)." },
  { eCSV,               '\0', "csv",           nullptr, nullptr, "Format output as CSV, for easy import into a spreadsheet."  },
  { eQuiet,             'q',  "quiet",         nullptr, nullptr, "Don't print any intermediate text, just the final results." },
  { eOutFile,           'o',  "out-file",      "%s",    nullptr, "Write the results to a file rather than stdout."            },
  { vh::kUnknownOption, '\0', nullptr,         nullptr, nullptr, nullptr                                                      }
};


namespace vh {

  typedef bool (*ParserFunc)(const char* filename, double& parsingSecsOut);


  static bool prewarm_parser(const char* filename);
  static bool parse_with_minipbrt(const char* filename, double& parsingSecsOut);
  static bool parse_with_minipbrt_threaded(const char* filename, double& parsingSecsOut);
  static bool parse_with_pbrtparser(const char* filename, double& parsingSecsOut);


  enum ParserID {
    ePBRTParser,
    eMiniPBRT,
    eThreaded,
  };


  static const char* kParserNames[] = {
    "pbrt-parser",
    "minipbrt",
    "threaded",
  };


  static const ParserFunc kParsers[] = {
    parse_with_pbrtparser,
    parse_with_minipbrt,
    parse_with_minipbrt_threaded,
  };
  static const uint32_t kNumParsers = sizeof(kParsers) / sizeof(kParsers[0]);


  struct Result {
    std::string filename;
    double secs[kNumParsers];
    bool ok[kNumParsers];
  };


  static int file_open(FILE** f, const char* filename, const char* mode)
  {
  #ifdef _WIN32
    return fopen_s(f, filename, mode);
  #else
    *f = fopen(filename, mode);
    return (*f != nullptr) ? 0 : errno;
  #endif
  }


  // Read the PBRT file and any additional files it references, so that they're
  // likely to be in the OS's disk cache when we run the parsing performance
  // tests. We use minipbrt for this, but just read any PLY files into memory
  // and discard them rather than loading them into triangle meshes.
  static bool prewarm_parser(const char* filename)
  {
    minipbrt::Loader loader;
    bool ok = loader.load(filename);
    if (!ok) {
      return false;
    }

    const size_t bufLen = 1024 * 1024 - 1;
    char* buffer = new char[bufLen + 1];
    buffer[bufLen] = '\0';

    const minipbrt::Scene* scene = loader.borrow_scene();
    for (uint32_t i = 0, endI = uint32_t(scene->shapes.size()); i < endI; i++) {
      if (scene->shapes[i]->type() == minipbrt::ShapeType::PLYMesh) {
        const minipbrt::PLYMesh* plymesh = dynamic_cast<const minipbrt::PLYMesh*>(scene->shapes[i]);
        FILE* f = nullptr;
        if (file_open(&f, plymesh->filename, "rb") != 0) {
          continue;
        }
        while (fread(buffer, sizeof(char), bufLen, f) == bufLen) {
          continue;
        }
        fclose(f);
      }
    }

    delete[] buffer;
    return true;
  }


  static bool parse_with_minipbrt(const char* filename, double& parsingSecsOut)
  {
    Timer timer(true); // true --> autostart the timer.

    minipbrt::Loader loader;
    bool ok = loader.load(filename);
    if (ok) {
      minipbrt::Scene* scene = loader.take_scene();
      ok = scene->load_all_ply_meshes();
      delete scene;
    }

    timer.stop();
    parsingSecsOut = timer.elapsedSecs();

    return ok;
  }


  static bool parse_with_minipbrt_threaded(const char* filename, double& parsingSecsOut)
  {
    Timer timer(true); // true --> autostart the timer.

    minipbrt::Loader loader;
    bool ok = loader.load(filename);
    if (ok) {
      minipbrt::Scene* scene = loader.take_scene();

      std::vector<uint32_t> plymeshes;
      plymeshes.reserve(scene->shapes.size());
      for (size_t i = 0, endI = scene->shapes.size(); i < endI; i++) {
        if (scene->shapes.at(i)->type() == minipbrt::ShapeType::PLYMesh) {
          plymeshes.push_back(uint32_t(i));
        }
      }

      std::atomic_uint nextMesh(0);
      const uint32_t endMesh = uint32_t(plymeshes.size());
      const uint32_t numThreads = std::thread::hardware_concurrency();

      std::vector<std::thread> loaderThreads;
      loaderThreads.reserve(numThreads);

      for (uint32_t i = 0; i < numThreads; i++) {
        loaderThreads.push_back(std::thread([scene, &plymeshes, &nextMesh, endMesh]() {
          uint32_t mesh = nextMesh++;
          while (mesh < endMesh) {
            scene->to_triangle_mesh(plymeshes.at(mesh));
            mesh = nextMesh++;
          }
        }));
      }

      for (std::thread& th : loaderThreads) {
        th.join();
      }

      delete scene;
    }

    timer.stop();
    parsingSecsOut = timer.elapsedSecs();

    return ok;
  }


  static bool parse_with_pbrtparser(const char* filename, double& parsingSecsOut)
  {
    Timer timer(true); // true --> autostart the timer.

    bool ok = true;
    try {
      pbrt::Scene::SP pbrtScene = pbrt::importPBRT(filename);
      ok = pbrtScene != nullptr;
    }
    catch (const std::exception& e) {
      fprintf(stderr, "pbrt-parser error: %s\n", e.what());
      ok = false;
    }
    
    timer.stop();
    parsingSecsOut = timer.elapsedSecs();

    return ok;
  }


  static void parse(const char* filename, const bool enabled[kNumParsers], bool prewarm, bool verbose, Result& result)
  {
    result.filename = filename;
    if (prewarm) {
      prewarm_parser(filename);
    }
    for (uint32_t p = 0; p < kNumParsers; p++) {
      if (!enabled[p]) {
        continue;
      }
      if (verbose) {
        printf("Parsing %s with %s...\n", filename, kParserNames[p]);
        fflush(stdout);
      }
      result.ok[p] = kParsers[p](filename, result.secs[p]);
    }
  }


  static uint32_t first_enabled(const bool enabled[kNumParsers])
  {
    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (enabled[i]) {
        return i;
      }
    }
    return kNumParsers;
  }


  static bool multiple_parsers_enabled(const bool enabled[kNumParsers])
  {
    bool foundOne = false;
    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (enabled[i]) {
        if (foundOne) {
          return true;
        }
        foundOne = true;
      }
    }
    return false;
  }


  static void print_header(FILE* out, int filenameWidth, const bool enabled[kNumParsers], uint32_t baseline)
  {
    bool showSpeedup = baseline < kNumParsers;

    fprintf(out, "| %-*s ", filenameWidth, "Filename");
    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (!enabled[i]) {
        continue;
      }
      if (showSpeedup && i != baseline) {
        fprintf(out, "| %12s (Speedup) ", kParserNames[i]);
      }
      else {
        fprintf(out, "| %12s ", kParserNames[i]);
      }
    }
    fprintf(out, "|\n");

    fprintf(out, "| :");
    for (int i = 0; i < filenameWidth - 1; i++) {
      fputc('-', out);
    }
    fputc(' ', out);

    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (!enabled[i]) {
        continue;
      }
      if (showSpeedup && i != baseline) {
        fprintf(out, "| ---------------------: ");
      }
      else {
        fprintf(out, "| -----------: ");
      }
    }

    fprintf(out, "|\n");
  }


  static void print_result(FILE* out, const Result& result, int filenameWidth, const bool enabled[kNumParsers], uint32_t baseline)
  {
    bool showSpeedup = baseline < kNumParsers;

    fprintf(out, "| %-*s ", filenameWidth, result.filename.c_str());

    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (!enabled[i]) {
        continue;
      }
      if (showSpeedup && i != baseline) {
        if (result.ok[i] && result.ok[baseline]) {
          double speedup = result.secs[baseline] / result.secs[i];
          fprintf(out, "| %12.3lf (%6.2lfx) ", result.secs[i], speedup);
        }
        else if (result.ok[i] && !result.ok[baseline]) {
          fprintf(out, "| %12.3lf           ", result.secs[i]);
        }
        else {
          fprintf(out, "| %12s           ", "failed");
        }
      }
      else {
        if (result.ok[i]) {
          fprintf(out, "| %12.3lf ", result.secs[i]);
        }
        else {
          fprintf(out, "| %12s ", "failed");
        }
      }
    }

    fprintf(out, "|\n");
  }


  static void print_results(FILE* out, const std::vector<Result> results, const bool enabled[kNumParsers])
  {
    int filenameWidth = 0;
    for (const Result& result : results) {
      int newWidth = static_cast<int>(result.filename.size());
      if (newWidth > filenameWidth) {
        filenameWidth = newWidth;
      }
    }

    uint32_t baseline = multiple_parsers_enabled(enabled) ? first_enabled(enabled) : kNumParsers;

    print_header(out, filenameWidth, enabled, baseline);
    for (const Result& result : results) {
      print_result(out, result, filenameWidth, enabled, baseline);
    }
  }


  static void print_results_as_csv(FILE* out, const std::vector<Result> results, const bool enabled[kNumParsers])
  {
    fprintf(out, "\"Filename\"");
    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (enabled[i]) {
        fprintf(out, ", \"%s\"", kParserNames[i]);
      }
    }
    fprintf(out, "\n");

    fprintf(out, "\n");

    for (const Result& result : results) {
      fprintf(out, "\"%s\"", result.filename.c_str());

      for (uint32_t i = 0; i < kNumParsers; i++) {
        if (!enabled[i]) {
          continue;
        }

        if (result.ok[i]) {
          fprintf(out, ", %lf", result.secs[i]);
        }
        else {
          fprintf(out, ", \"failed\"");
        }
      }

      fprintf(out, "\n");
    }
  }


  static bool has_extension(const char* filename, const char* ext)
  {
    int j = int(strlen(ext));
    int i = int(strlen(filename)) - j;
    if (i <= 0 || filename[i - 1] != '.') {
      return false;
    }
    return strcmp(filename + i, ext) == 0;
  }

} // namespace vh


int main(int argc, char** argv)
{
  using namespace vh;

  bool enabled[kNumParsers] = { true, true, true };
  bool prewarm = true;
  bool printAsCSV = false;
  bool verbose = true;

  const char* outfile = nullptr;

  int argi = 1;
  MatchedOption match;
  while (argi < argc) {
    if (match_option(argc, argv, argi, options, match)) {
      switch (match.id) {
      case eHelp:
        print_help(argv[0], options);
        return 0;

      case eVersion:
        print_version(kTitle, kMajorVersion, kMinorVersion);
        return 0;

      case eNoMiniPBRT:
        enabled[eMiniPBRT] = false;
        break;

      case eNoPBRTParser:
        enabled[ePBRTParser] = false;
        break;

      case eNoPrewarm:
        prewarm = false;
        break;

      case eCSV:
        printAsCSV = true;
        break;

      case eQuiet:
        verbose = false;
        break;

      case eOutFile:
        parse_option(argc, argv, argi, &options[match.id], &outfile);
        break;

      // Handle other options here

      default:
        break;
      }

      remove_matched_option(argc, argv, argi, match);
    }
    else {
      ++argi;
    }
  }
  // All matched options have been removed from argv by this point and argc
  // has been adjusted accordingly. argv[0] is still the executable name.

  if (argc <= 1) {
    fprintf(stderr, "No input files provided.\n");
    return EXIT_SUCCESS;
  }

  // Process the files, building up a table of results. We don't just print the
  // results out as we go because pbrt-parser prints some intermediate output
  // which messes up our formatting.
  const int kFilenameBufferLen = 16 * 1024 - 1;
  char* filenameBuffer = new char[kFilenameBufferLen + 1];
  filenameBuffer[kFilenameBufferLen] = '\0';

  std::vector<std::string> filenames;
  for (int i = 1; i < argc; i++) {
    if (has_extension(argv[i], "txt")) {
      FILE* f = nullptr;
      if (file_open(&f, argv[i], "r") == 0) {
        while (fgets(filenameBuffer, kFilenameBufferLen, f)) {
          filenames.push_back(filenameBuffer);
          while (filenames.back().back() == '\n') {
            filenames.back().pop_back();
          }
        }
        fclose(f);
      }
      else {
        fprintf(stderr, "Failed to open %s\n", argv[i]);
      }
    }
    else {
      filenames.push_back(argv[i]);
    }
  }

  if (verbose) {
    printf("Parsing these files:\n");
    for (const std::string& filename : filenames) {
      printf("  %s\n", filename.c_str());
    }
  }

  std::vector<Result> results(filenames.size(), Result{});
  for (size_t i = 0; i < filenames.size(); i++) {
    parse(filenames[i].c_str(), enabled, prewarm, verbose, results[i]);
  }
  if (verbose) {
    printf("Parsing complete!\n\n");
  }

  FILE* out = stdout;
  if (outfile != nullptr) {
    if (file_open(&out, outfile, "w") != 0) {
      fprintf(stderr, "Failed to open output file %s for writing\n", outfile);
      return EXIT_FAILURE;
    }
  }

  if (printAsCSV) {
    print_results_as_csv(out, results, enabled);
  }
  else {
    print_results(out, results, enabled);
  }

  if (out != stdout) {
    fclose(out);
  }

  return EXIT_SUCCESS;
}
