<?php

$fileName = __DIR__ . '/6.txt';

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
        foreach ($points as $k => $coords) {
            $d[$k] = abs($coords[0] - $x) + abs($coords[1] - $y);
        }

        $m = min($d);

        $i = array_keys($d, $m);
        if (count($i) === 1) {
            $map[$y][$x] = $i[0];
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

for ($y = 0; $y < $maxy + 1; $y++) {
    for ($x = 0; $x < $maxx + 1; $x++) {
        if (($y !== 0 && $y !== $maxy && ($x === 0 || $x === $maxx))
            || ($x !== 0 && $x !== $maxx && ($y === 0 || $y === $maxy))
        ) {
            if (isset($map[$y][$x]) && isset($counts[$map[$y][$x]])) {
                unset($counts[$map[$y][$x]]);
            }
        }
    }
}

var_dump(max($counts));
