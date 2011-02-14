<?php
// $Id$

/**
 * @file
 * Provide filter handler for wf_field.module fields
 */
class views_handler_filter_field_wf_field_list extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options ) ) {
      module_load_include('inc', 'wf_field');
      $this->value_options = _wf_field_get_states($this->definition['field_name']);
    }
    return $this->value_options;
  }
  
}
