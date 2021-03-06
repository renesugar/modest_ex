
#include "test_includes.h"
int main(int argc, const char * argv[])
{
  // initialize all of Erl_Interface
  erl_init(NULL, 0);
  
  html_workspace_t* w = html_init();

  const char *html = "<div>Hello <p>World</p></div>";
  vec_eterm_t term_array; 
  eterm_vec_init(&term_array);
  get_text(w, html, &term_array);
  char* result = eterm_vec_join(&term_array, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "Hello |World") != 0){
    eterm_vec_destroy(&term_array);
    free(result);
    html_destroy(w);
    TEST_ERROR
    return 1;
  }
  eterm_vec_destroy(&term_array);
  free(result);

  html = "<div><p>Hello World</p></div>";
  eterm_vec_init(&term_array);
  select_and_get_text(w, html, "div p", &term_array);
  result = eterm_vec_join(&term_array, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "Hello World") != 0){
    eterm_vec_destroy(&term_array);
    free(result);
    html_destroy(w);
    TEST_ERROR
    return 1;
  }
  eterm_vec_destroy(&term_array);
  free(result);

  html = "<div><p>Hello</p></div>";
  const char *scope = "body_children";
  eterm_vec_init(&term_array);
  select_and_set_text(w, html, "div p", "World", scope, &term_array);
  result = eterm_vec_join(&term_array, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<div><p>World</p></div>") != 0){
    eterm_vec_destroy(&term_array);
    free(result);
    html_destroy(w);
    TEST_ERROR
    return 1;
  }
  eterm_vec_destroy(&term_array);
  free(result);

  html = "<div><p></p></div>";
  scope = "body_children";
  eterm_vec_init(&term_array);
  select_and_set_text(w, html, "div p", "Hello World", scope, &term_array);
  result = eterm_vec_join(&term_array, "|");
  printf("-> %s\n", result);
  if(strcmp(result, "<div><p>Hello World</p></div>") != 0){
    eterm_vec_destroy(&term_array);
    free(result);
    html_destroy(w);
    TEST_ERROR
    return 1;
  }
  eterm_vec_destroy(&term_array);
  free(result);

  html_destroy(w);
  printf("ok\n");
  return 0;
}