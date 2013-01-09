<?php

/**
 * Implements hook_views_data_alter().
 */
function vc_vote_views_data_alter(&$data) {
  // Flag content links.
  $data['flag_content']['vc_vote'] = array(
    'title' => t('Vote link'),
    'help' => t('Display vote/cancel link.'),
    'field' => array(
      'handler' => 'vc_vote_handler_field_ops',
    ),
  );

  $flags = flag_get_flags();
  foreach (array_keys($flags) as $flag_name) {
    if (!variable_get("vc_vote_{$flag_name}", FALSE)) continue;
    $table = NULL;

    switch ($flags[$flag_name]->content_type) {
      case 'node':
        $table = &$data['node'];
        break;
      case 'user':
        $table = &$data['users'];
        break;
    }

    if ($table) {
      $table['vc_votes_vote'] = $table['votingapi_vote'];
      $table['vc_votes_vote']['relationship']['title'] .= " › {$flags[$flag_name]->title}";
      $table['vc_votes_vote']['relationship']['extra'][] = array('field' => 'tag', 'value' => "vc_vote_{$flag_name}");

      $table['vc_votes_cache'] = $table['votingapi_cache'];
      $table['vc_votes_cache']['relationship']['title'] .= " › {$flags[$flag_name]->title}";
      $table['vc_votes_cache']['relationship']['extra'][] = array('field' => 'tag', 'value' => "vc_vote_{$flag_name}");
      # $table['vc_votes_cache']['handler'] = 'vc_vote_views_handler_relationship';
    }
  }
}
