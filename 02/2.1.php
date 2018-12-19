<?php

$fileName = __DIR__ . '/2.txt';

$file = file($fileName);

$two = 0;
$three = 0;

foreach ($file as $line) {
    if (empty($line)) {
        continue;
    }

    $matches = [];
    for ($i = 0; $i < strlen($line); $i++) {
        if ($line[$i] === PHP_EOL) {
            continue;
        }

        if (empty($matches[$line[$i]])) {
            $matches[$line[$i]] = 0;
        }
        $matches[$line[$i]]++;
    }

    $twom = false;
    $threem = false;
    foreach ($matches as $c) {
        if ($c === 2 && !$twom) {
            $two++;
            $twom = true;
        } elseif ($c === 3 && !$threem) {
            $three++;
            $threem = true;
        }
    }
}

var_dump($two * $three); die();

