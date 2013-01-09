<?php

/**
 * Menu callback
 */
function vc_vote_page_callback($flag, $entity, $action) {
  global $user;

  $info = entity_get_info($flag->content_type);

  // Shorten up the variables that affect the behavior of this page.
  $js = isset($_REQUEST['js']);

  // Specifically $_GET to avoid getting the $_COOKIE variable by the same key.
  $has_js = isset($_GET['has_js']);

  switch ($action) {
    case 'vote':
      $votes[] = array(
        'entity_type' => $flag->content_type,
        'entity_id' => $entity->{$info['entity keys']['id']},
        'value_type' => 'points',
        'value' => 1,
        'tag' => "vc_vote_{$flag->name}",
      );
      votingapi_set_votes($votes);
      break;

    case 'cancel':
      $criteria['tag'] = "vc_vote_{$flag->name}";
      $criteria['uid'] = $user->uid;
      $votes = votingapi_select_votes($criteria);
      votingapi_delete_votes($votes);
      break;

    default:
      $exception = new Exception(t('Unknow action'));
      watchdog_exception('vc_vote', $exception);
      throw $exception;
      break;
  }

    $flag->flag_short = '';
    $flag->flag_long = '';
    $flag->flag_message = '';
    $flag->flag_message = '';
    $flag->unflag_message = '';
    $flag->link_type = 'toggle';
    #kpr($flag); exit;

  if ($js) {
    drupal_add_http_header('Content-Type', 'text/javascript; charset=utf-8');

    $entity_id = $entity->{$info['entity keys']['id']};
    $action    = $action == 'vote' ? 'cancel' : 'vote';
    $url       = url("vc-vote/{$flag->name}/{$entity_id}/{$action}/nojs");
    $label     = $action == 'vote' ? t('Vote') : t('Cancel');

    $output = flag_build_javascript_info($flag, $entity_id);
    $output['newLink'] = preg_replace('/href="[^"]+"/', 'href="' . $url . '"', $output['newLink']);
    $output['newLink'] = preg_replace('/rel="nofollow">.+<\/a>/', "rel=\"nofollow\">{$label}</a>", $output['newLink']);
    print drupal_json_encode($output);
    drupal_exit();
  }
  else {
    $path = $info['uri callback']($entity);
    drupal_goto($path['path']);
  }
}
