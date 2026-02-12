<?php
$file = isset($_GET['file']) ? basename($_GET['file']) : '';
$allowedFiles = ['CV2025.pdf', 'BA510NA CI.pdf'];

if (in_array($file, $allowedFiles) && file_exists($file)) {
    header('Content-type: application/pdf');
    header('Content-Disposition: inline; filename="'.$file.'"');
    readfile($file);
    exit;
} else {
    die('Fichier PDF introuvable');
}
?>
