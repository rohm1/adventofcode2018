<?php

$fileName = __DIR__ . '/5.txt';

$lengths = [];

for ($j = ord('a'); $j <= ord('z'); $j++) {
    $f = trim(file_get_contents($fileName));
    $f = str_replace(chr($j), '', $f);
    $f = str_replace(chr($j - 32), '', $f);

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

    $lengths[$j] = strlen($f);
}

var_dump(min($lengths));
