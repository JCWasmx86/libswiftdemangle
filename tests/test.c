#include <assert.h>
#include <swiftdemangle.h>
#include <stdlib.h>
#include <string.h>

int main(int    argc,
         char **argv)
{
  const char *input = "$s12MesonAnalyze12TypeAnalyzerC1t0A3AST0C9NamespaceCSgvsTq";
  size_t input_len = strlen (input);
  char *output = swift_demangle (input, input_len, NULL, NULL, 0);
  assert (strcmp("method descriptor for MesonAnalyze.TypeAnalyzer.t.setter : Swift.Optional<MesonAST.TypeNamespace>", output) == 0);
  free (output);
  return 0;
}
