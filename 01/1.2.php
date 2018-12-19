<?php

$fileName = __DIR__ . '/1.txt';

$file = file($fileName);

$fs = [];
$f = 0;

while (true) {
    foreach ($file as $line) {
        if (empty($line)) {
            continue;
        }

        $f += (int) $line;

        if (array_key_exists($f, $fs)) {
            var_dump($f); die();
        } else {
            $fs[$f] = true;
        }
    }

    reset($file);
}
