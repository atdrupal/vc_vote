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
      vc_vote_vote($flag, $entity->{$info['entity keys']['id']});
      break;

    case 'cancel':
      vc_vote_cancel($flag, $entity->{$info['entity keys']['id']}, $user->uid);
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
    if (isset($_GET['destination'])) {
      drupal_goto($_GET['destination']);
    }

    $path = $info['uri callback']($entity);
    drupal_goto($path['path']);
  }
}
