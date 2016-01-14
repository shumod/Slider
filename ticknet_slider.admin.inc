<?php

function ticknet_slider_settings() {
    $default = array(
        'width' => '628',
        'height' => '218',
        'button_width' => '95',
        'bottom_height' => '45',
        'timer' => 5000,
        'opacity' => 6,
        'buttonText' => 'GET TICKETS',
    );
    $setting = variable_get('ts_setting', $default);
    $options = array(
        '1000' => 1,
        '2000' => 2,
        '3000' => 3,
        '4000' => 4,
        '5000' => 5,
        '6000' => 6,
        '7000' => 7,
        '8000' => 8,
        '9000' => 9,
        '10000' => 10,
    );
    $opacity = array();
    for ($i = 0; $i <= 10; $i++) {
        $opacity[$i] = $i * 0.1;
    }
    $form['width'] = array(
        '#type' => 'textfield',
        '#title' => t('Slider width'),
        '#default_value' => $setting['width'],
        '#description' => t('Please insert the slider block width'),
        '#field_suffix' => 'px',
        '#size' => 4,
        '#maxlength' => 4,
    );
    $form['height'] = array(
        '#type' => 'textfield',
        '#title' => t('Slider height'),
        '#default_value' => $setting['height'],
        '#description' => t('Please insert the slider block height'),
        '#field_suffix' => 'px',
        '#size' => 4,
        '#maxlength' => 4,
    );
    $form['button_width'] = array(
        '#type' => 'textfield',
        '#title' => t('Slider button width'),
        '#default_value' => $setting['button_width'],
        '#description' => t('Please insert the slider block width'),
        '#field_suffix' => 'px',
        '#size' => 4,
        '#maxlength' => 4,
    );
    $form['bottom_height'] = array(
        '#type' => 'textfield',
        '#title' => t('Slider bottom height'),
        '#default_value' => $setting['bottom_height'],
        '#description' => t('Please insert the slider bottom block height'),
        '#field_suffix' => 'px',
        '#size' => 4,
        '#maxlength' => 4,
    );
    $form['timer'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Timer'),
        '#default_value' => $setting['timer'],
        '#description' => t('Please select how often change page of slider block')
    );
    $form['opacity'] = array(
        '#type' => 'select',
        '#options' => $opacity,
        '#title' => t('Opacity'),
        '#default_value' => $setting['opacity'],
        '#description' => t('Please select opacity bottom block')
    );

    $form['buttonText'] = array(
        '#type' => 'textfield',
        '#size' => 10,
        '#title' => t('Button Text'),
        '#default_value' => $setting['buttonText'],
        '#description' => t('Please enter slider block button text')
    );
    $form['slider_save'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#name' => 'save-setting',
        '#submit' => array('ticknet_slider_setting_submit'),
    );

    return $form;
}

function ticknet_slider_setting_submit($form, $form_state) {
    $setting = array();
    $setting['width'] = $form_state['values']['width'];
    $setting['height'] = $form_state['values']['height'];
    $setting['button_width'] = $form_state['values']['button_width'];
    $setting['bottom_height'] = $form_state['values']['bottom_height'];
    $setting['timer'] = $form_state['values']['timer'];
    $setting['opacity'] = $form_state['values']['opacity'];
    $setting['buttonText'] = $form_state['values']['buttonText'];
    variable_set('ts_setting', $setting);
}

function ticknet_slider_edit(&$form_state, $page="") {
//    dprint_r($page);
//
    $tmp_path = file_directory_path() . '/' . variable_get('ticknet_slider_path', 'ticknet_slider');
    file_check_directory($tmp_path, FILE_CREATE_DIRECTORY);
    $tmp_path = file_directory_path() . '/' . variable_get('ticknet_slider_path', 'ticknet_slider') . '/images';
    file_check_directory($tmp_path, FILE_CREATE_DIRECTORY);
    $form['#prefix'] = '<div id="slider-preview">' . theme('ticknet_slider_block', $page->img_path, $page->body, $page->url) . '</div>';
    $form['eid'] = array('#type' => hidden, '#value' => $page->eid);
//    $form['fid'] = array('#type' => hidden, '#value' => $page->fid);

    $form['status'] = array(
        '#type' => 'checkbox',
        '#title' => t('Status'),
        '#default_value' => $form_state['values']['status'] ? $form_state['values']['status'] : $page->status,
        '#description' => t('Please check to enable this page')
    );
    $form['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title '),
        '#default_value' => $form_state['values']['title'] ? $form_state['values']['title'] : $page->title,
        '#description' => t('Please enter title for slider block page')
    );
    $form['body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body text'),
        '#default_value' => $form_state['values']['body'] ? $form_state['values']['body'] : $page->body,
        '#description' => t('Please enter body for slider block page'),
        '#id' => 'bodyedit',
    );
    $form['format'] = filter_form();
    $form['url'] = array(
        '#type' => 'textfield',
        '#title' => t('URL'),
        '#default_value' => $form_state['values']['url'] ? $form_state['values']['url'] : $page->url,
        '#description' => t('Please enter url for slider block page')
    );
    $form['weight'] = array(
        '#type' => 'value',
        '#value' => $page->weight,
    );
    $form['filepath'] = array(
        '#type' => 'value',
        '#value' => $page->img_path,
    );
    $form['upload'] = array(
        '#type' => 'file',
        '#title' => t('Upload image file'),
        '#size' => 40,
        '#description' => t("Please, add image with 3:1 ratio size. Valid file extensions is jpg, png, gif. Maximum upload size 150 KB."),
        '#element_validate' => array('ticknet_slider_image_upload'),
    );
    $form['form_preview'] = array(
        '#type' => 'value',
        '#value' => 0,
    );
    $form['buttons'] = array();
    $form['buttons']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#weight' => 5,
        '#validate' => array('ticknet_slider_validate_save'),
    );
    $form['buttons']['preview'] = array(
        '#type' => 'submit',
        '#value' => t('Preview'),
        '#weight' => 10,
        '#submit' => array('ticknet_slider_edit_submit'),
        '#validate' => array('ticknet_slider_validate_preview'),
    );

    $form['#attributes']['enctype'] = 'multipart/form-data';
    return $form;
}

function ticknet_slider_edit_submit($form, &$form_state) {
    $imgfile = $form_state['values']['filepath'];
    empty($form_state['values']['file']->filepath) ? $imgpath = $imgfile : $imgpath = $form_state['values']['file']->filepath;
    if (empty($file)) {
        $file = file_save_upload('upload', $validators, $tmp_path);

        if (!empty($form_state['values']['file']->filepath)) {
            file_set_status($file, FILE_STATUS_PERMANENT);
            //delete old file
            file_delete($imgfile);
            db_query("DELETE FROM {files} WHERE filepath = '%s'", $imgfile);
        }
    }
    $page = array(
        "eid" => $form_state['values']['eid'],
        "title" => $form_state['values']['title'],
        "body" => $form_state['values']['body'],
        "url" => $form_state['values']['url'],
        "img_path" => $imgpath,
        "fid" => $form_state['values']['file']->fid,
        "status" => $form_state['values']['status'],
        "created" => time(),
        "weight" => $form_state['values']['weight'],
    );
//    dprint_r($form_state['values']);
    drupal_write_record("ticknet_slider_data", $page, array('eid'));
    if ($form_state['values']['form_preview'] == 0) {
        $form_state['redirect'] = 'admin/content/ticknet_slider/list';
    } else {
//        dprint_r($page);
//        $form_state['rebuild'] = TRUE;
//        cache_clear_all();
        $form_state['redirect'] = 'admin/content/ticknet_slider/edit/' . $page['eid'];
    }
}

function ticknet_slider_add(&$form_state) {
//image
    $tmp_path = file_directory_path() . '/' . variable_get('ticknet_slider_path', 'ticknet_slider');
    file_check_directory($tmp_path, FILE_CREATE_DIRECTORY);
    $tmp_path = file_directory_path() . '/' . variable_get('ticknet_slider_path', 'ticknet_slider') . '/images';
    file_check_directory($tmp_path, FILE_CREATE_DIRECTORY);
    $form['#attributes']['enctype'] = 'multipart/form-data';
    $form['status'] = array(
        '#type' => 'checkbox',
        '#title' => t('Status'),
        '#default_value' => $form_state['values']['status'],
        '#description' => t('Please check to enable this page')
    );
    $form['title'] = array(
        '#type' => 'textarea',
        '#title' => t('Title text'),
        '#default_value' => $form_state['values']['title'],
        '#description' => t('Please enter title for slider block page')
    );
    $form['body'] = array(
        '#type' => 'textarea',
        '#title' => t('Body text'),
        '#default_value' => $form_state['values']['body'],
        '#description' => t('Please enter body for slider block page'),
    );
    $form['format'] = filter_form();
    $form['url'] = array(
        '#type' => 'textfield',
        '#title' => t('URL'),
        '#default_value' => $form_state['values']['url'],
        '#description' => t('Please enter url for slider block page')
    );

    $form['upload'] = array(
        '#type' => 'file',
        '#title' => t('Upload image file'),
        '#size' => 40,
        '#description' => t("Please, add image with 3:1 ratio size. Valid file extensions is jpg, png, gif. Maximum upload size 150 KB."),
        '#element_validate' => array('ticknet_slider_image_upload'),
    );
    $form['form_preview'] = array(
        '#type' => 'value',
        '#value' => 0,
    );
    $form['buttons'] = array();
    $form['buttons']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#weight' => 5,
        '#validate' => array('ticknet_slider_validate_save'),
    );
    $form['buttons']['preview'] = array(
        '#type' => 'submit',
        '#value' => t('Preview'),
        '#weight' => 10,
        '#submit' => array('ticknet_slider_add_submit'),
        '#validate' => array('ticknet_slider_validate_preview'),
    );
    return $form;
}

function ticknet_slider_image_upload($element, &$form_state) {
    if (empty($_FILES['files']['name']['upload']) && !empty($form_state['values']['filepath']))
        return FALSE;
//    dprint_r($_FILES['files']);
    // define upload field name
    // NOTE: this should match the name of your form file field
    $fieldName = 'upload';

    // If a file was uploaded, process it.
    if (isset($_FILES['files']) && is_uploaded_file($_FILES['files']['tmp_name'][$fieldName])) {
        $validators = array(
            'file_validate_is_image' => array(),
            'file_validate_size' => array(150 * 1024),
        );
        // attempt to save the uploaded file
//        $path = drupal_get_path('module', 'ticknet_slider') . '/images';
        $path = file_directory_path() . '/ticknet_slider/images';
        $file = file_save_upload($fieldName, $validators, $path);
        file_set_status($file, FILE_STATUS_PERMANENT);
        // set error if file was not uploaded
        if (!$file) {
            form_set_error($fieldName, 'Error uploading file.');
            return;
        }

        // set files to form_state, to process when form is submitted
        $form_state['values']['file'] = $file;
    } else {
        // set error
        form_set_error($fieldName, 'Error uploading file.');
        return;
    }
}

function ticknet_slider_validate_save($form, &$form_state) {
    $form_state['values']['form_preview'] = 0;
}

function ticknet_slider_validate_preview($form, &$form_state) {
    $form_state['values']['form_preview'] = 1;
}

function ticknet_slider_add_submit($form, &$form_state) {
    $sql = "SELECT AUTO_INCREMENT FROM information_schema.TABLES WHERE table_name='{ticknet_slider_data}'";
    $eid = db_result(db_query($sql));
    $page = array(
        "title" => $form_state['values']['title'],
        "body" => $form_state['values']['body'],
        "url" => $form_state['values']['url'],
        "img_path" => $form_state['values']['file']->filepath,
        "status" => $form_state['values']['status'],
        "fid" => $form_state['values']['file']->fid,
        "created" => time(),
        "weight" => 0
    );
    drupal_write_record("ticknet_slider_data", $page);
//    dprint_r($form_state['values']);
    if ($form_state['values']['form_preview'] == 0) {
        $form_state['redirect'] = 'admin/content/ticknet_slider/list';
    } else {
        $form_state['redirect'] = 'admin/content/ticknet_slider/edit/' . $eid;
    }
}

function ticknet_slider_ticknet_slider_operations() {
    $operations = array(
        'enable' => array(
            'label' => t('Enable page'),
        ),
        'disable' => array(
            'label' => t('Disable page'),
        ),
        'delete' => array(
            'label' => t('Delete'),
        ),
    );
    return $operations;
}

function ticknet_slider_list($form_state) {
    if (isset($form_state['values']['operation'])) {
        switch ($form_state['values']['operation']) {
            case 'delete':
                return ticknet_slider_delete_confirm($form_state, array_filter($form_state['values']['pages']));
                break;
            case 'enable':
                return ticknet_slider_enable_confirm($form_state, array_filter($form_state['values']['pages']));
                break;
            case 'disable':
                return ticknet_slider_disable_confirm($form_state, array_filter($form_state['values']['pages']));
                break;
        }
    }

    $form['options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Update options'),
        '#prefix' => '<div class="container-inline">',
        '#suffix' => '</div>',
    );
    $options = array();
    foreach (module_invoke_all('ticknet_slider_operations') as $operation => $array) {
        $options[$operation] = $array['label'];
    }
    $form['options']['operation'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => 'approve',
    );
    $form['options']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Update'),
        '#submit' => array('ticknet_slider_list_submit'),
    );


    $header = array(
        array('data' => t('Title'), 'field' => 'title'),
        array('data' => t('Status'), 'field' => 'status'),
        array('data' => t('Operations')),
    );
    $result = db_query('SELECT ff.* FROM {ticknet_slider_data} ff ORDER BY weight');
//    $result = pager_query('SELECT ff.* FROM {ticknet_slider_data} ff' . tablesort_sql($header), variable_get('ticknet_slider_data_list', 50), 0, NULL, NULL);

    while ($page = db_fetch_object($result)) {
        $form[$page->eid]['weight'] = array('#type' => 'weight', '#delta' => 10, '#default_value' => $page->weight);
        $form[$page->eid]['title'] = array('#value' => $page->title);
        if ($page->status) {
            $status = t('Enabled');
        } else {
            $status = t('Disabled');
        }
        $form[$page->eid]['status'] = array('#value' => $status);
        $form[$page->eid]['eid'] = array('#type' => 'value', '#value' => $page->eid);
        $form[$page->eid]['edit'] = array('#value' => l(t('Edit'), 'admin/content/ticknet_slider/edit/' . $page->eid));
        $pages[$page->eid] = '';
    }//while
    $form['reorder'] = array(
        '#type' => 'submit',
        '#value' => t('Reorder items'),
        '#name' => 'reorder',
    );
    $form['pages'] = array(
        '#type' => 'checkboxes',
        '#options' => $pages
    );
//    $form['pager'] = array('#value' => theme('pager', NULL, variable_get('ticknet_slider_data_list', 50), 0));
    return $form;
}

function ticknet_slider_list_submit($form, &$form_state) {

    $operations = module_invoke_all('ticknet_slider_operations');
    $operation = $operations[$form_state['values']['operation']];
    $pages = array_filter($form_state['values']['pages']);

    if ($function = $operation['callback']) {
        if (isset($operation['callback arguments'])) {
            $args = array_merge(array($pages), $operation['callback arguments']);
        } else {
            $args = array($pages);
        }
        call_user_func_array($function, $args);

        cache_clear_all();
    } else {
        $form_state['rebuild'] = TRUE;
    }
}

function ticknet_slider_enable_confirm(&$form_state, $pages) {
    $form['pages'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
    foreach ($pages as $eid => $value) {
        $page = ticknet_slider_page_load($eid);
        $form['pages'][$eid] = array(
            '#type' => 'hidden',
            '#value' => $eid,
            '#prefix' => '<li>',
            '#suffix' => check_plain($page->title) . "</li>\n",
        );
    }
    $form['operation'] = array('#type' => 'hidden', '#value' => 'enable');
    $form['#submit'][] = 'ticknet_slider_enable_confirm_submit';
    return confirm_form($form,
            t('Are you sure you want to enable these items?'),
            'admin/content/ticknet_slider/list', '',
            t('Enable for all'), t('Cancel'));
}

function ticknet_slider_enable_confirm_submit($form, &$form_state) {
    if ($form_state['values']['confirm']) {
        foreach ($form_state['values']['pages'] as $eid => $value) {
            db_query("UPDATE {ticknet_slider_data} SET status='1' WHERE eid = '%d'", $eid);
        }
        drupal_set_message(t('The items have been enabled.'));
    }
    $form_state['redirect'] = 'admin/content/ticknet_slider/list';
    return;
}

function ticknet_slider_disable_confirm(&$form_state, $pages) {

    $form['pages'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
    foreach ($pages as $eid => $value) {
        $page = ticknet_slider_page_load($eid);
        $form['pages'][$eid] = array(
            '#type' => 'hidden',
            '#value' => $eid,
            '#prefix' => '<li>',
            '#suffix' => check_plain($page->title) . "</li>\n",
        );
    }
    $form['operation'] = array('#type' => 'hidden', '#value' => 'disable');
    $form['#submit'][] = 'ticknet_slider_disable_confirm_submit';
    return confirm_form($form,
            t('Are you sure you want to disable these items?'),
            'admin/content/ticknet_slider/list', '',
            t('Disable for all'), t('Cancel'));
}

function ticknet_slider_disable_confirm_submit($form, &$form_state) {
    if ($form_state['values']['confirm']) {
        foreach ($form_state['values']['pages'] as $eid => $value) {
            db_query("UPDATE {ticknet_slider_data} SET status=0 WHERE eid = '%d'", $eid);
        }
        drupal_set_message(t('The items have been disabled.'));
    }
    $form_state['redirect'] = 'admin/content/ticknet_slider/list';
    return;
}

function ticknet_slider_delete_confirm(&$form_state, $pages) {

    $form['pages'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
    // array_filter returns only elements with TRUE values
    foreach ($pages as $eid => $value) {
        $page = ticknet_slider_page_load($eid);
        $form['pages'][$eid] = array(
            '#type' => 'hidden',
            '#value' => $eid,
            '#prefix' => '<li>',
            '#suffix' => check_plain($page->title) . "</li>\n",
        );
    }
    $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
    $form['#submit'][] = 'ticknet_slider_delete_confirm_submit';
    return confirm_form($form,
            t('Are you sure you want to delete these items?'),
            'admin/content/ticknet_slider/list', t('This action cannot be undone.'),
            t('Delete all'), t('Cancel'));
}

function ticknet_slider_delete_confirm_submit($form, &$form_state) {
    if ($form_state['values']['confirm']) {
        foreach ($form_state['values']['pages'] as $eid => $value) {
            $page = ticknet_slider_page_load($eid);
            db_query("DELETE FROM {ticknet_slider_data} WHERE eid = '%d'", $eid);
//            $file = db_fetch_object(db_query("SELECT * FROM {files} WHERE filepeth = '%s'", $page->img_path));
            file_delete($page->img_path);
            db_query("DELETE FROM {files} WHERE fid = '%d'", $page->fid);
        }
        drupal_set_message(t('The items have been deleted.'));
    }
    $form_state['redirect'] = 'admin/content/ticknet_slider/list';
    return;
}
