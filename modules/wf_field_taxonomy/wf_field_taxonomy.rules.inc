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
  $events = _wf_field_build_entity_transtion_rules('taxonomy_term', 'wf_field_taxonomy', array(
    'taxonomy_term' => array('type' => 'taxonomy_term', 'label' => t('updated taxonomy term')),
    'acting_account' => array('type' => 'user', 'label' => t('acting user')),
  ));
  return $events;
}
