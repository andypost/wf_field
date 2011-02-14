<?php
// $Id$
 
/**
 * @file
 * Provide views data and handlers for wf_field.module
 */


/**
 * Implements hook_field_views_data().
 *
 * Views integration for wf_field fields. Adds a special lookup for wf_fields 
 * 
 * @see field_views_field_default_views_data()
 */
function wf_field_field_views_data($field) {
  $data = field_views_field_default_views_data($field);
  foreach ($data as $table_name => $table_data) {
    foreach ($table_data as $field_name => $field_data) {
      if (!in_array($field_name, array('table', 'entity_id', 'revision_id'))) {
        $data[$table_name][$field_name]['filter']['handler'] = 'views_handler_filter_field_wf_field_list';
      }
    }
  }
  return $data;
}

