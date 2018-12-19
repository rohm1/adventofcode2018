<?php

$fileName = __DIR__ . '/2.txt';

$f = file($fileName);

foreach ($f as $line) {
    $line = trim($line);
    $ff = file($fileName);
    foreach ($ff as $line2) {
        $line2 = trim($line2);
        if (empty($line2)) {
            continue;
        }

        $same = '';
        for ($i = 0; $i < strlen($line); $i++) {
            if ($line[$i] === $line2[$i]) {
                $same .= $line[$i];
            }
        }

        if (strlen($line) === strlen($same) + 1) {
            var_dump($same);
            die();
        }
    }
}
