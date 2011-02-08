<?php
// $Id$

/**
 * @file
 *
 * Page callbacks.
 */

/**
 * Form builder; Form for confirming a transition triggering
 *
 * @param $node
 *   $node
 * @param $field_name
 *   $field_name
 * @param $target_state
 *   $target_state
 *
 * @ingroup forms
 * @see wf_field_trigger_transition_submit()
 */
function wf_field_trigger_transition($form, $form_state, $node, $field_name, $target_state) {
  // let's make sure we have a valid field
  $instance = _wf_field_get_wf_field_instance('node', $field_name, $node->type);
  if (!$instance) {
    drupal_set_message( t('Invalid workflow field given') );
    return drupal_not_found();
  }
  if (!wf_field_is_transition_allowed($node, $field_name, $target_state)) {
    drupal_set_message( t('This transition is not allowed'), 'error' );
    return drupal_access_denied();
  }
  // if we have a valid token skip the confirmation form
  // @todo:  
  if ( isset($_REQUEST['token']) && $_REQUEST['token']) {
    if ( drupal_get_token($_GET['q']) == $_REQUEST['token'] ) {
      module_load_include('inc', 'wf_field');
      wf_field_transition_execute($node, $field_name, $target_state, array('access_check' => FALSE) );
      drupal_goto( isset($_REQUEST['destination']) ? $_REQUEST['destination'] : null );
    }
  } 
  else {
    $form_state['node'] = $node;
    $form_state['field_name'] = $field_name;
    $form_state['target_state'] = $target_state;
    $state_label = wf_field_get_current_state_label($target_state, $instance );
    $info = wf_field_get_current_transition_options($node, $instance, array('complete_info'=>true));
    $transition_label = $info[ $target_state ]['label'];
    
    
    return confirm_form(
        $form,
        t('Please confirm the transition %transition_label', array('%transition_label' => $transition_label)),
        isset($_REQUEST['destination']) ? drupal_parse_url( $_REQUEST['destination'] ) : '',
        t('Do you want to transition !title to the state %state_label?', array('!title' => l($node->title, 'node/' . $node->nid), '%state_label' => $state_label)),
        t('@transition_label @title', array('@transition_label'=>$transition_label, '@title'=>$node->title) ),  t('Cancel')
      );
  }
}

/**
 * Submit handler to transitioning
 */
function wf_field_trigger_transition_submit($form, &$form_state) {
  // execute the transition - we'll be redirected to ?destination
  module_load_include('inc', 'wf_field');
  wf_field_transition_execute( $form_state['node'], $form_state['field_name'], $form_state['target_state'] );
}
