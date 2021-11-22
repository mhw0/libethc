#include <locale.h>
#include <glib.h>
#include <ethc.h>

static void say_hello_test_func() { }

int main(int argc, char *argv[]) {
  setlocale (LC_ALL, "");
  g_test_init (&argc, &argv, NULL);

  g_test_add_func("/say_hello", say_hello_test_func);

  return g_test_run();
}
