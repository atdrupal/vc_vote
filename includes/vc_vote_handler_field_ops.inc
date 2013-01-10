<?php

class vc_vote_handler_field_ops extends flag_handler_field_ops {
  function render($values) {
    if (!($flag = $this->get_flag())) {
      return t('Missing flag'); // get_flag() itself will print a more detailed message.
    }

    $content_id = $values->{$this->aliases['content_id']};
    $is_flagged = $values->{$this->aliases['is_flagged']};

    if (!variable_get("vc_vote_{$flag->name}", FALSE)) {
      // Flag does not apply to this content.
      return;
    }

    $criteria['tag'] = "vc_vote_{$flag->name}";
    $criteria['entity_id'] = $content_id;
    $votes = votingapi_select_votes($criteria);
    $action = empty($votes) ? 'vote' : 'cancel';
    $url    = url("vc-vote/{$flag->name}/{$content_id}/{$action}/nojs", array('query' => array(drupal_get_destination())));
    $label  = empty($votes) ? t('Vote') : t('Cancel');

    $flag->link_type = ''; # 'toggle';
    $output = $flag->theme($is_flagged ? 'unflag' : 'flag', $content_id);
    $output = preg_replace('/href="[^"]+"/', 'href="' . $url . '"', $output);
    $output = preg_replace('/rel="nofollow">.+<\/a>/', "rel=\"nofollow\">{$label}</a>", $output);
    return $output;
  }
}
