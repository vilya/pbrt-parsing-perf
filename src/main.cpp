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

  struct Result {
    const char* filename;
    double minipbrtSecs;
    double pbrtparserSecs;
    bool minipbrtOK;
    bool pbrtparserOK;
  };


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


  static void print_header(int filenameWidth, bool minipbrtEnabled, bool pbrtparserEnabled)
  {
    printf("%-*s", filenameWidth, "Filename");
    if (minipbrtEnabled) {
      printf("  %12s", "minipbrt");
    }
    if (pbrtparserEnabled) {
      printf("  %12s", "pbrt-parser");
    }
    if (minipbrtEnabled && pbrtparserEnabled) {
      printf("  %12s", "speedup");
    }
    printf("\n");

    for (int i = 0; i < filenameWidth; i++) {
      fputc('-', stdout);
    }
    if (minipbrtEnabled) {
      printf("  ------------");
    }
    if (pbrtparserEnabled) {
      printf("  ------------");
    }
    if (minipbrtEnabled && pbrtparserEnabled) {
      printf("  ------------");
    }
    printf("\n");

    fflush(stdout);
  }


  static void print_result(const Result* result, int filenameWidth, bool minipbrtEnabled, bool pbrtparserEnabled)
  {
    printf("%-*s", filenameWidth, result->filename);

    if (minipbrtEnabled) {
      if (result->minipbrtOK) {
        printf("  %12.3lf", result->minipbrtSecs);
      }
      else {
        printf("  %12s", "failed");
      }
    }

    if (pbrtparserEnabled) {
      if (result->pbrtparserOK) {
        printf("  %12.3lf", result->pbrtparserSecs);
      }
      else {
        printf("  %12s", "failed");
      }
    }

    if (minipbrtEnabled && pbrtparserEnabled) {
      if (result->minipbrtOK && result->pbrtparserOK) {
        double speedup = result->pbrtparserSecs / result->minipbrtSecs;
        printf("  %11.2lfx", speedup);
      }
      else {
        printf("  %12c", '-');
      }
    }

    printf("\n");
  }


  static void print_results(const Result results[], int numResults, bool minipbrtEnabled, bool pbrtparserEnabled)
  {
    int filenameWidth = 0;
    for (int i = 0; i < numResults; i++) {
      int newWidth = static_cast<int>(strlen(results[i].filename));
      if (newWidth > filenameWidth) {
        filenameWidth = newWidth;
      }
    }

    print_header(filenameWidth, minipbrtEnabled, pbrtparserEnabled);
    for (int i = 0; i < numResults; i++) {
      print_result(results + i, filenameWidth, minipbrtEnabled, pbrtparserEnabled);
    }
  }


  static void print_results_as_csv(const Result results[], int numResults, bool minipbrtEnabled, bool pbrtparserEnabled)
  {
    printf("\"Filename\"");
    if (minipbrtEnabled) {
      printf(", \"minipbrt\"");
    }
    if (pbrtparserEnabled) {
      printf(", \"pbrt-parser\"");
    }
    printf("\n");

    printf("\n");

    for (int i = 0; i < numResults; i++) {
      printf("\"%s\"", results[i].filename);
      if (minipbrtEnabled) {
        if (results->minipbrtOK) {
          printf(", %lf", results->minipbrtSecs);
        }
        else {
          printf(", \"failed\"");
        }
      }
      if (pbrtparserEnabled) {
        if (results->minipbrtOK) {
          printf(", %lf", results->minipbrtSecs);
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

  bool minipbrtEnabled = true;
  bool pbrtparserEnabled = true;
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
        minipbrtEnabled = false;
        break;

      case eNoPBRTParser:
        pbrtparserEnabled = false;
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
  int numResults = argc - 1;
  Result* results = new Result[numResults];
  for (int i = 1; i < argc; i++) {
    Result* result = &results[i - 1];
    result->filename = argv[i];
    if (minipbrtEnabled) {
      printf("Parsing %s with minipbrt...\n", result->filename);
      fflush(stdout);
      result->minipbrtOK = parse_with_minipbrt(result->filename, result->minipbrtSecs);
    }
    if (pbrtparserEnabled) {
      printf("Parsing %s with pbrt-parser...\n", result->filename);
      fflush(stdout);
      result->pbrtparserOK = parse_with_pbrtparser(result->filename, result->pbrtparserSecs);
    }
  }
  printf("Parsing complete!\n\n");

  if (printAsCSV) {
    print_results_as_csv(results, numResults, minipbrtEnabled, pbrtparserEnabled);
  }
  else {
    print_results(results, numResults, minipbrtEnabled, pbrtparserEnabled);
  }
  return EXIT_SUCCESS;
}
