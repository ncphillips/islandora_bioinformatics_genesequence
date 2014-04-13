<?php

/**
 * @file
 * Functions deriving fasta files for objects.
 */
 
/**
 * Copies the Fasta file to the file system.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 *
 * @return array|stdClass
 *   FALSE when the object doesn't have the OBJ, an array otherwise.
 *
 *	@author Quintin Dawson
 */
 
 function islandora_lab_object_genesequence_create_fasta_file(AbstractObject $object, $force = FALSE) {
  $return = '';
  if (!isset($object['FASTA']) || (isset($object['FASTA']) && $force === TRUE)) {
    $original_file = islandora_lab_object_genesequence_copy_fasta_datastream($object);
    if ($original_file === FALSE) {
      $return  = array(
    'success' => FALSE,
    'messages' => array(
      array(
      'message' => t('Could not create file for fasta %s. No fasta file was uploaded.'),
      'message_sub' => array('%s' => $object->id),
      'type' => 'watchdog',
      'severity' => WATCHDOG_ERROR,
      ),
    ),
    );
  } 
  else {
      $return = array(
        'success' => TRUE,
        'messages' => array(
        array(
          'message' => t('The FASTA file was created successfully for @pid!'),
          'message_sub' => array('@pid' => $object->id),
          'type' => 'dsm',
        ),
      ),
    );
  }
    return $return;
  }
}
 
 
function islandora_lab_object_genesequence_copy_fasta_datastream(AbstractObject $object) {

  $mime_detect = new MimeDetect();
  if (!isset($object['FASTA'])) {
    return FALSE;
  }
  $ext = $mime_detect->getExtension($object['FASTA']->mimeType);
  $file_name = str_replace(':', '-', $object->id);

  // Create a file object we can save.
  //$file_uri = file_create_filename("{$file_name}FASTA.{$ext}", 'temporary://');
  $file_uri = file_create_filename("{$file_name}FASTA.{$ext}", 'public://fasta/');
  
  $file = new stdClass();
  $file->uri = $file_uri;
  $file->filename = $file_name;
  $file->filemime = $object['FASTA']->mimeType;
 
  
  $file->status = 0;
  $object['FASTA']->getContent($file_uri);
  $original_file = file_save($file);
  
  //testing
  //$file_copy($original_file, 'public://fasta/' . $file->file_name . $ext);
  
  return array(
    'file' => $original_file,
    'extension' => $ext,
    'filename' => $file_name,
  );
}