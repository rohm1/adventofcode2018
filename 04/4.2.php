<?php

$fileName = __DIR__ . '/4.txt';

$file = file($fileName);

$events = [];
foreach ($file as $line) {
    if (preg_match('/^\[(.+)\] (.+)$/U', $line, $match)) {
        $events[$match[1]] = trim($match[2]);
    }
}

ksort($events);

$guards = [];
$g = -1;
$start = 0;

foreach ($events as $date => $event) {
    if (preg_match('/#(\d+)/', $event, $match)) {
        $g = $match[1];
        if (!array_key_exists($g, $guards)) {
            $guards[$g] = [];
        }
    }

    if ($event === 'falls asleep') {
        $d = explode(':', $date);
        $dd = $d[0];
        $m = (int) $d[1];
        do {
            $key = $dd . ':' . str_pad($m, 2, 0, STR_PAD_LEFT);
            if (isset($events[$key]) && $events[$key] === 'wakes up') {
                break;
            }

            if (!isset($guards[$g][$m])) {
                $guards[$g][$m] = 0;
            }

            $guards[$g][$m]++;
            $m++;
        } while (true);
    }
}

$max = 0;

foreach ($guards as $g => &$data) {
    if (empty($data)) {
        continue;
    }
    $m = max($data);
    if ($m > $max) {
        $max = $m;
        $mg = $g;
    }
}

var_dump($mg * array_search(max($guards[$mg]), $guards[$mg]));
