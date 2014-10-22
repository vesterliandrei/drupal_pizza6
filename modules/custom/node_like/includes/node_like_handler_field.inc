<?php

/**
 * @file
 * Contains the class to extend views by providing a new custom field which adds
 * count node likes for a speciffic CT.
 */

/**
 * Base field handler that has no options and renders an unformatted field.
 *
 * @ingroup views_field_handlers
 */
class node_like_handler_field extends views_handler_field {
  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   */
  function render($values) {
    $value = $this->get_value($values);

    if (!$value) {
      $value = 0;
    }

    // Check to see if hiding should happen before adding prefix and suffix.
    if ($this->options['hide_empty'] && empty($value) && ($value !== 0 || $this->options['empty_zero'])) {
      $value = '';
    }

    return $this->sanitize_value($this->options['prefix'], 'xss')
      . $this->sanitize_value($value)
      . $this->sanitize_value($this->options['suffix'], 'xss');
  }

  /**
   * Render a numeric value as a size.
   *
   * @ingroup views_field_handlers
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['prefix'] = array('default' => '', 'translatable' => TRUE);
    $options['suffix'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    $form['prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Prefix'),
      '#default_value' => $this->options['prefix'],
      '#description' => t('Text to put before the number, such as currency symbol.'),
    );

    $form['suffix'] = array(
      '#type' => 'textfield',
      '#title' => t('Suffix'),
      '#default_value' => $this->options['suffix'],
      '#description' => t('Text to put after the number, such as currency symbol.'),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Called to add the field to a query.
   */
  function query() {
    // Get all view fields and check if field "likes" is present. If not then alter
    // query in the same logic as field handler.
    $fields = $this->query->fields;
    if (!in_array($this->field, $fields)) {
      $this->ensure_my_table();

      // Add the field.
      $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
      // Get base table name.
      $base_table = $this->query->base_table;
      // Get field formula
      $formula = $this->get_formula('node_like', $base_table);
      $this->field_alias = $this->query->add_field(null, $formula, $this->field, $params);
      $this->query->distinct = TRUE;
    }
  }

  /**
   * Add formula for getting field data.
   */
  function get_formula($join_table, $base_table) {
    return "(SELECT COUNT($join_table.nid) FROM $join_table WHERE $join_table.nid = $base_table.nid GROUP BY $join_table.nid)";
  }
}
