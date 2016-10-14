<?php 
//$Id$
/**
 *  @file
 *  Template for displaying the voting widget
 */
 
  echo "in plusone-widget.tpl.php<br/>";

  drupal_add_js(drupal_get_path('module', 'plusone') .'/plusone.js');
  drupal_add_css(drupal_get_path('module', 'plusone') .'/plusone.css');

  $output = '<div class="plusone-widget">';
  $output .= '<div class="score">'. $total .'</div>';

  $output .= '<div class="vote">';

  if ($is_author) {
    // User is author; not allowed to vote.
    $output .= t('Votes');
  }
  elseif ($voted > 0) {
    // User already voted; not allowed to vote again.
    $output .= t('You voted');
  }
  else {
    // User is eligible to vote.
    $output .= l(t('Vote'), "plusone/vote/$nid", array(
      'attributes' => array('class' => 'plusone-link')
      ));
  }

  $output .= '</div>'; // Close div with class "vote".
  $output .= '</div>'; // Close div with class "plusone-widget".
 
  print $output;

