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
function wf_field_taxonomy_rules_event_info() {
  module_load_include('inc', 'wf_field');
  $events = _wf_field_build_entity_transtion_rules('taxonomy', 'wf_field_taxonomy', array(
    'taxonomy' => array('type' => 'taxonomy_term', 'label' => t('updated taxonomy item')),
    'acting_account' => array('type' => 'user', 'label' => t('acting user')),
  ));
  return $events;
}
