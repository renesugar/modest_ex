#include <stdio.h>
#include <stdlib.h>

#include "erl_interface.h"
#include "ei.h"
#include "eterm_array.h"

#include "modest_html.h"

void slice_selected(html_workspace_t* w, const char* html, const char* selector, int start, int end, const char* delimiter, eterm_array_t* term_array)
{
  int tree_index = html_parse_tree(w, html, strlen(html));
  int selector_index = html_prepare_selector(w, selector, strlen(selector));
  int collection_index  = html_select(w, tree_index, "html", selector_index);

  collection_index = html_slice(w, collection_index, start, end);

  // int buffer_index = html_serialize_tree(w, tree_index, scope_name);
  int buffer_index = html_serialize_collection(w, collection_index);
  html_vec_str_t* buffer = html_get_buffer(w, buffer_index);
  int i; char* val;
  html_vec_foreach(buffer, val, i) {
    if(term_array != NULL && val != NULL && strlen(val) > 0) {
#if 0
      char* copy;
      html_string_copy(val, copy);
      printf("%d (%d): %s\n", i, buffer->length, copy);
      eterm_array_push(term_array, erl_mk_binary(copy, strlen(copy)));
#endif
#if 1
      printf("%d (%d): '%s'\n", i, buffer->length, val);
      eterm_array_push(term_array, erl_mk_binary(val, strlen(val)));
      printf("ok\n");
#endif
    }  
  }
  printf("return\n");
}

ETERM* handle_slice(ErlMessage* emsg)
{
  ETERM* response = NULL;
  ETERM* pattern = erl_format("{slice, Html, Selector, StartIndex, EndIndex, Delimiter}");

  if (erl_match(pattern, emsg->msg)) {
    ETERM* html_term = erl_var_content(pattern, "Html");
    ETERM* selector_term = erl_var_content(pattern, "Selector");
    ETERM* start_term = erl_var_content(pattern, "StartIndex");
    ETERM* end_term = erl_var_content(pattern, "EndIndex");
    ETERM* delimiter_term = erl_var_content(pattern, "Delimiter");
    
    char* html = (char*)ERL_BIN_PTR(html_term);
    char* selector = (char*)ERL_BIN_PTR(selector_term);
    char* start = (char*)ERL_BIN_PTR(start_term);
    char* end = (char*)ERL_BIN_PTR(end_term);
    char* delimiter = (char*)ERL_BIN_PTR(delimiter_term);

    html_workspace_t* workspace = html_init();
    eterm_array_t* term_array = eterm_array_init();
    slice_selected(workspace, html, selector, atoi(start), atoi(end), delimiter, term_array);
    ETERM* term_list = eterm_array_to_list(term_array);
    response = erl_format("{slice, ~w}", term_list);

    // free allocated resources
    eterm_array_destroy(term_array);
    erl_free_term(term_list);
    html_destroy(workspace);
    erl_free_term(html_term);
    erl_free_term(selector_term);
    erl_free_term(start_term);
    erl_free_term(end_term);
    erl_free_term(delimiter_term);
  }

  erl_free_term(pattern);
  return response;
}