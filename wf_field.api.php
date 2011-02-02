<?php
/**
 * @file
 * Hooks provided by the wf_field API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Hook invoked before transition rules get executed. 
 * If you return WF_FIELD_DENY (à la nodeaccess) the node_save() will throw an exception.
 * We pray that there is a rollback.   
 */
function wf_field_hook_wf_field_before_transition_rules($info) {
  // $info is populated like this:
  $info = array('field_instance'=>$instance, 'old_value'=>$old_value, 'new_value'=>$new_value, 'author'=>$author, 'node'=>$node, 'unsaved_node'=>$node, 'user'=>$user);
}

/** 
 * Hook invoked after transition rules get executed. The ship has sailed - no rollback no more.
 */
function wf_field_hook_wf_field_after_transition_rules($info) {
  // $info is populated like this:
  $info = array('field_instance'=>$instance, 'old_value'=>$old_value, 'new_value'=>$new_value, 'author'=>$author, 'node'=>$node, 'unsaved_node'=>$node, 'user'=>$user);
}


/** 
 * Alter hook called when the transitions for an *concrete* node instance are built/prepared. 
 */
function wf_field_hook_wf_field_transitions_alter(&$transition_states, $info) {
  // $info is populated like this:
  $info = array('object'=>$state_or_node, 'field_instance'=>$instance, 'options'=>$options);  
}

