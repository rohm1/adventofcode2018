<?php

$fileName = __DIR__ . '/5.txt';

$f = trim(file_get_contents($fileName));

while (true) {
    $found = false;
    for ($i = 0; $i < strlen($f) - 1; $i++) {
        $c = $f[$i];
        $n = $f[$i + 1];
        if (ord($n) === ord($c) + 32 || ord($n) + 32 === ord($c)) {
            $f = str_replace($c . $n, '', $f);
            $f = str_replace($n . $c, '', $f);
            $found = true;
            break;
        }
    }

    if (!$found) {
        break;
    }
}

var_dump(strlen($f));
