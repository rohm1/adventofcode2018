<?php

$fileName = __DIR__ . '/1.txt';

$file = file($fileName);

$f = 0;

foreach ($file as $line) {
    if (empty($line)) {
        continue;
    }

    $f += (int) $line;
}

var_dump($f);
