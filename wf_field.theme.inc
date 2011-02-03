<?php
// $Id$

/**
 * @file
 *
 * Theming functions
 */

/**
 * Returns HTML for a list of recent comments to be displayed in the comment block.
 *
 * @ingroup themeable
 */
function theme_wf_field_transition_block( &$variables ) {
  $grouped_transitions = $variables['grouped_transitions'];
  $node = $variables['node'];
  $confirm_form = isset($variables['confirm_form']) && $variables['confirm_form'];
  $query = array('destination' => $_GET['q']); 
  foreach ($grouped_transitions as $nr => $info) {
    foreach ($info['transitions'] as $target_state => $transition_label) {
      $url = 'node/' . $node->nid . '/wf_field_trigger_transition/' . $info['instance']['field_name'] . '/' . $target_state;
      // CSRF protection
      if (!$confirm_form) {
        $query['token'] = drupal_get_token($url) ;
      }      
      $items[] = l( $transition_label, $url, array('query' => $query) );
    }
  }

  if ($items) {
    return array(
    	'#theme' => 'item_list',
      '#items' => $items,
    ); 
  }
  else {
    return t('No transitions available.');
  }
}
