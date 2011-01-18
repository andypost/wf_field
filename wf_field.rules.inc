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
  $field_instances = _wf_field_get_all_instances();
  // Build up all the possible events for the workflow fields.
  $events = array();
  foreach ($field_instances as $instance) {
    $transitions = wf_field_get_transitions( $instance, array('all_transitions' => TRUE, 'ignore_permissions' => TRUE) );
    foreach ($transitions as $key => $transition) {
      $events['wf_field_transition_' . $instance['field_name'] . '_' . $key] = array(
        'group' => t('Workflow Field'),
        'label' => t('@field - On Transition: !transition', array('@field' => $instance['field_name'], '!transition' => $transition)),
        'module' => 'wf_field',
        'arguments' => array(
          'node' => array('type' => 'node', 'label' => t('updated content')),
          'unsaved_node' => array('type' => 'node', 'label' => t('unsaved content')),
          'account' => array('type' => 'user', 'label' => t('acting user')),
          'author' => array('type' => 'user', 'label' => t('node author')),
        ),
      );
    }
    // on entering & on leaving states
    $states = _wf_field_get_states($instance);
    foreach ($states as $key => $label ) {
      $events['wf_field_in_' . $instance['field_name'] . '_' . $key] = array(
        'group' => t('Workflow Field'),
        'label' => t('@field - On entering state (=inward): !label', array('@field' => $instance['field_name'], '!label' => $label)),
        'module' => 'wf_field',
        'arguments' => array(
          'node' => array('type' => 'node', 'label' => t('updated content')),
          'unsaved_node' => array('type' => 'node', 'label' => t('unsaved content')),
          'account' => array('type' => 'user', 'label' => t('acting user')),
          'author' => array('type' => 'user', 'label' => t('node author')),
        ),
      );
      $events['wf_field_out_' . $instance['field_name'] . '_' . $key] = array(
        'group' => t('Workflow Field'),
        'label' => t('@field - On leaving state (=outward): !label', array('@field' => $instance['field_name'], '!label' => $label)),
        'module' => 'wf_field',
        'arguments' => array(
          'node' => array('type' => 'node', 'label' => t('updated content')),
          'unsaved_node' => array('type' => 'node', 'label' => t('unsaved content')),
          'account' => array('type' => 'user', 'label' => t('acting user')),
          'author' => array('type' => 'user', 'label' => t('node author')),
        ),
      );
    }
  }

  return $events;
}
