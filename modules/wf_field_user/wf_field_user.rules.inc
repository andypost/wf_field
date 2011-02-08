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
function wf_field_user_rules_event_info() {
  module_load_include('inc', 'wf_field');
  $events = _wf_field_build_entity_transtion_rules('user', 'wf_field_user', array(
    'account' => array('type' => 'user', 'label' => t('updated account')),
    'acting_account' => array('type' => 'user', 'label' => t('acting user')),
  ));
  return $events;
}
