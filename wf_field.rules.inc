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
  // by default we're only implementing for nodes
  module_load_include('inc', 'wf_field');
  $events = _wf_field_build_entity_transtion_rules('node', 'wf_field', array(
    'node' => array('type' => 'node', 'label' => t('updated content')),
    'account' => array('type' => 'user', 'label' => t('acting user')),
    'author' => array('type' => 'user', 'label' => t('node author')),
  ));
  return $events;
}

/**
 * Implements hook_rules_event_info().
 *
 * This function declares all possible workflow transition actions 
 */
function wf_field_rules_action_info() {
  // here we're only implementing actions for the node entity-type
  // actions: set field {wf_field_name} of {node} to workflow state {wf_state_id}  
  return array();
}

/**
 * Implements hook_rules_event_info().
 *
 * This function declares all possible workflow transition actions 
 */
function wf_field_rules_condition_info() {
  // here we're only implementing conditions for the node entity-type
  // conditions: node has field {wf_field_name} to workflow state {wf_state_id}  
  return array();
}

