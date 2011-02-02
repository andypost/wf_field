<?php
/**
 * Form builder; Form for confirming a transition
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
      wf_field_transition_execute($node, $field_name, $target_state, array('access_check'=>false) );
      drupal_goto( $_REQUEST['destination'] );
    }
  } else {
    $form_state['node'] = $node;
    $form_state['field_name'] = $field_name;
    $form_state['target_state'] = $target_state;
  
    $transition_label = wf_field_get_current_state_label($target_state, $instance );
    
    return confirm_form(
        $form,
        t('Please confirm the transition %transition_label', array('%transition_label'=>$transition_label)),
        drupal_parse_url( $_REQUEST['destination'] ),
        t('Do you want to execute the workflow transition %transition_label on !title?', array('!title'=>l($node->title, 'node/' . $node->nid), '%transition_label'=>$transition_label)),
        $transition_label,  t('Cancel')
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
