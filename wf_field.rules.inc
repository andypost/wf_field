<?php
// $Id$

/**
 * @file
 * rules hooks
 */


/**
 * Implements hook_rules_event_info().
 *
 * This function declares all the possible workflow transition events.
 */
function wf_field_rules_event_info() {
  module_load_include('inc', 'wf_field');
  $events = _wf_field_build_entity_transtion_rules('node', 'wf_field', array(
    'node' => array('type' => 'node', 'label' => t('updated content')),
    'account' => array('type' => 'user', 'label' => t('acting user')),
    'author' => array('type' => 'user', 'label' => t('node author')),
  ));
  return $events;
}
