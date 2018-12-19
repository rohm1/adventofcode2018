<?php

$fileName = __DIR__ . '/3.txt';

$file = file($fileName);

$map = [];
$claimed = 0;
$maxx = 0;
$maxy = 0;

foreach ($file as $line) {
    if (preg_match('/^#(?<id>\d+) @ (?<left>\d+),(?<top>\d+): (?<width>\d+)x(?<height>\d+)$/', $line, $match)) {
        for ($y = $match['top']; $y < $match['top'] + $match['height']; $y++) {
            $maxy = max($maxy, $y);
            for ($x = $match['left']; $x < $match['left'] + $match['width']; $x++) {
                $maxx = max($maxx, $x);
                if (isset($map[$y][$x])) {
                    if ($map[$y][$x] !== 'X') {
                        $claimed++;
                        $map[$y][$x] = 'X';
                    }
                } else {
                    $map[$y][$x] = 'o';
                }
            }
        }
    } else {
        var_dump($line); die();
    }
}

foreach ($file as $line) {
    $overlap = false;
    if (preg_match('/^#(?<id>\d+) @ (?<left>\d+),(?<top>\d+): (?<width>\d+)x(?<height>\d+)$/', $line, $match)) {
        for ($y = $match['top']; $y < $match['top'] + $match['height']; $y++) {
            for ($x = $match['left']; $x < $match['left'] + $match['width']; $x++) {
                if ($map[$y][$x] === 'X') {
                    $overlap = true;
                    break 2;
                }
            }
        }
    } else {
        var_dump($line); die();
    }

    if (!$overlap) {
        var_dump($match['id']); die();
    }
}

//for ($y = 0; $y <= $maxy; $y++) {
//    for ($x = 0; $x <= $maxx; $x++) {
//        echo $map[$y][$x] ?? '.';
//    }
//    echo PHP_EOL;
//}
