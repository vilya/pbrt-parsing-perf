#include <minipbrt.h>
#include <pbrtParser/Scene.h>
#include <vh_cmdline.h>
#include <vh_time.h>

#include <cstdio>
#include <string>


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
  eNoPBRTParser,
  eCSV,
};

static const vh::CommandLineOption options[] = {
  { eHelp,              'h',  "help",          nullptr, nullptr, "Print this help message and exit."                         },
  { eVersion,           'v',  "version",       nullptr, nullptr, "Print the application version and exit."                   },
  { eNoMiniPBRT,        '\0', "no-minipbrt",   nullptr, nullptr, "Disable minpbrt parsing."                                  },
  { eNoPBRTParser,      '\0', "no-pbrtparser", nullptr, nullptr, "Disable pbrt-parser parsing."                              },
  { eCSV,               '\0', "csv",           nullptr, nullptr, "Format output as CSV, for easy import into a spreadsheet." },
  { vh::kUnknownOption, '\0', nullptr,         nullptr, nullptr, nullptr                                                     }
};


namespace vh {

  typedef bool (*ParserFunc)(const char* filename, double& parsingSecsOut);


  static bool null_parser(const char* filename, double& parsingSecsOut);
  static bool parse_with_minipbrt(const char* filename, double& parsingSecsOut);
  static bool parse_with_pbrtparser(const char* filename, double& parsingSecsOut);


  enum ParserID {
    eMiniPBRT,
    ePBRTParser,
  };
  static const uint32_t kNumParsers = uint32_t(ePBRTParser) + 1;


  static const char* kParserNames[] = {
    "minipbrt",
    "pbrt-parser",
  };


  static const ParserFunc kParsers[] = {
    parse_with_minipbrt,
    parse_with_pbrtparser,
  };


  struct Result {
    std::string filename;
    double secs[kNumParsers];
    bool ok[kNumParsers];
  };


  // Doesn't do any parsing, just reads the whole file as quickly as possible.
  // This is just used to pre-warm the file cache before the other parsers run.
  static bool null_parser(const char* filename)
  {
    FILE* f = nullptr;
    if (fopen_s(&f, filename, "rb") != 0) {
      return false;
    }
    const size_t bufLen = 1024 * 1024 - 1;
    char* buffer = new char[bufLen + 1];
    buffer[bufLen] = '\0';
    while (fread(buffer, sizeof(char), bufLen, f) == bufLen) {
      continue;
    }
    delete[] buffer;
    fclose(f);
    return  true;
  }


  static bool parse_with_minipbrt(const char* filename, double& parsingSecsOut)
  {
    Timer timer(true); // true --> autostart the timer.

    minipbrt::Parser parser;
    bool ok = parser.parse(filename);
    
    timer.stop();
    parsingSecsOut = timer.elapsedSecs();

    return ok;
  }


  static bool parse_with_pbrtparser(const char* filename, double& parsingSecsOut)
  {
    Timer timer(true); // true --> autostart the timer.

    bool ok = true;
    try {
      pbrt::Scene::SP pbrtScene;
      pbrtScene = pbrt::importPBRT(filename);
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


  static void parse(const char* filename, const bool enabled[kNumParsers], Result& result)
  {
    result.filename = filename;
    null_parser(filename);
    for (uint32_t p = 0; p < kNumParsers; p++) {
      if (!enabled[p]) {
        continue;
      }
      printf("Parsing %s with %s...\n", filename, kParserNames[p]);
      fflush(stdout);
      result.ok[p] = kParsers[p](filename, result.secs[p]);
    }
  }


  static void print_header(int filenameWidth, const bool enabled[kNumParsers])
  {
    printf("%-*s", filenameWidth, "Filename");
    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (enabled[i]) {
        printf("  %12s", kParserNames[i]);
      }
    }

    if (enabled[eMiniPBRT] && enabled[ePBRTParser]) {
      printf("  %12s", "Speedup");
    }
    printf("\n");

    for (int i = 0; i < filenameWidth; i++) {
      fputc('-', stdout);
    }
    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (enabled[i]) {
        printf("  ------------");
      }
    }
    if (enabled[eMiniPBRT] && enabled[ePBRTParser]) {
      printf("  ------------");
    }
    printf("\n");

    fflush(stdout);
  }


  static void print_result(const Result& result, int filenameWidth, const bool enabled[kNumParsers])
  {
    printf("%-*s", filenameWidth, result.filename.c_str());

    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (!enabled[i]) {
        continue;
      }
      if (result.ok[i]) {
        printf("  %12.3lf", result.secs[i]);
      }
      else {
        printf("  %12s", "failed");
      }
    }

    if (enabled[eMiniPBRT] && enabled[ePBRTParser]) {
      if (result.ok[eMiniPBRT] && result.ok[ePBRTParser]) {
        double speedup = result.secs[ePBRTParser] / result.secs[eMiniPBRT];
        printf("  %11.2lfx", speedup);
      }
      else {
        printf("  %12c", '-');
      }
    }

    printf("\n");
  }


  static void print_results(const std::vector<Result> results, const bool enabled[kNumParsers])
  {
    int filenameWidth = 0;
    for (const Result& result : results) {
      int newWidth = static_cast<int>(result.filename.size());
      if (newWidth > filenameWidth) {
        filenameWidth = newWidth;
      }
    }

    print_header(filenameWidth, enabled);
    for (const Result& result : results) {
      print_result(result, filenameWidth, enabled);
    }
  }


  static void print_results_as_csv(const std::vector<Result> results, const bool enabled[kNumParsers])
  {
    printf("\"Filename\"");
    for (uint32_t i = 0; i < kNumParsers; i++) {
      if (enabled[i]) {
        printf(", \"%s\"", kParserNames[i]);
      }
    }
    printf("\n");

    printf("\n");

    for (const Result& result : results) {
      printf("\"%s\"", result.filename.c_str());

      for (uint32_t i = 0; i < kNumParsers; i++) {
        if (!enabled[i]) {
          continue;
        }

        if (result.ok[i]) {
          printf(", %lf", result.secs[i]);
        }
        else {
          printf(", \"failed\"");
        }
      }

      printf("\n");
    }
  }

} // namespace vh


int main(int argc, char** argv)
{
  using namespace vh;

  bool enabled[kNumParsers] = { true, true };
  bool printAsCSV = false;

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

      case eCSV:
        printAsCSV = true;
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
  std::vector<Result> results;
  results.reserve(static_cast<size_t>(argc - 1));

  const int kFilenameBufferLen = 16 * 1024 - 1;
  char* filenameBuffer = new char[kFilenameBufferLen + 1];
  filenameBuffer[kFilenameBufferLen] = '\0';

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '@') {
      FILE* f = nullptr;
      if (fopen_s(&f, argv[i] + 1, "r") == 0) {
        while (fgets(filenameBuffer, kFilenameBufferLen, f)) {
          results.push_back(Result{});
          parse(filenameBuffer, enabled, results.back());
        }
        fclose(f);
      }
    }
    else {
      results.push_back(Result{});
      parse(argv[i], enabled, results.back());
    }
  }
  printf("Parsing complete!\n\n");

  if (printAsCSV) {
    print_results_as_csv(results, enabled);
  }
  else {
    print_results(results, enabled);
  }
  return EXIT_SUCCESS;
}
