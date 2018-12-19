<?php

$fileName = __DIR__ . '/6.txt';
$maxDistance = 10000;

$lines = file($fileName);

$map = [];
$points = [];
$maxy = 0;
$maxx = 0;

foreach ($lines as $k => $line) {
    if (empty($line)) {
        continue;
    }

    $n = explode(',', $line);
    $x = (int) $n[0];
    $y = (int) $n[1];
    $points[$k] = [$x, $y];

    $maxy = max($maxy, $y);
    $maxx = max($maxx, $x);

    if (!isset($map[$y])) {
        $map[$y] = [];
    }

    $map[$y][$x] = $k;
}

for ($y = 0; $y < $maxy + 1; $y++) {
    for ($x = 0; $x < $maxx + 1; $x++) {
        $d = [];
        foreach ($points as $coords) {
            $d[] = abs($coords[0] - $x) + abs($coords[1] - $y);
        }

        $s = array_sum($d);
        if ($s < $maxDistance) {
            $map[$y][$x] = '#';
        }
    }
}

foreach ($points as $coords) {
    $map[$coords[1]][$coords[0]] = '#';
}

$mark_region = function (int $x, int $y, int $r) use (&$map, &$mark_region) {
    foreach ([[-1, 0], [0, -1], [0, 1], [1, 0]] as $coords) {
        if (isset($map[$y + $coords[1]][$x + $coords[0]]) && $map[$y + $coords[1]][$x + $coords[0]] === '#') {
            $map[$y + $coords[1]][$x + $coords[0]] = $r;
            $mark_region($x + $coords[0], $y + $coords[1], $r);
        }
    }
};

$r = 0;
for ($y = 0; $y < $maxy + 1; $y++) {
    for ($x = 0; $x < $maxx + 1; $x++) {
        if (isset($map[$y][$x]) && $map[$y][$x] === '#') {
            $map[$y][$x] = $r;
            $mark_region($x, $y, $r);
            $r++;
        }
    }
}

$counts = [];
for ($y = 0; $y < $maxy + 1; $y++) {
    for ($x = 0; $x < $maxx + 1; $x++) {
        if (isset($map[$y][$x])) {
            if (!isset($counts[$map[$y][$x]])) {
                $counts[$map[$y][$x]] = 0;
            }

            $counts[$map[$y][$x]]++;
        }
    }
}

//for ($y = 0; $y < $maxy + 1; $y++) {
//    for ($x = 0; $x < $maxx + 1; $x++) {
//        echo $map[$y][$x] ?? '.';
//    }
//
//    echo PHP_EOL;
//}

var_dump(max($counts));
