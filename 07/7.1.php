<?php

$fileName = __DIR__ . '/7.txt';

$lines = file($fileName);

$steps = [];
$stepEntity = [
    'done' => false,
    'deps' => [],
];

foreach ($lines as $line) {
    if (preg_match('/^Step ([A-Z]) must be finished before step ([A-Z]) can begin.$/', $line, $match)) {
        if (!isset($steps[$match[1]])) {
            $steps[$match[1]] = $stepEntity;
        }

        if (!isset($steps[$match[2]])) {
            $steps[$match[2]] = $stepEntity;
        }

        $steps[$match[2]]['deps'][] = $match[1];
    }
}

ksort($steps);

$runned = [];

do {
    $toRun = [];

    foreach ($steps as $step => $config) {
        if (!$config['done']) {
            $available = true;

            foreach ($config['deps'] as $d) {
                if (!$steps[$d]['done']) {
                    $available = false;
                    break;
                }
            }

            if ($available) {
                $toRun[] = $step;
            }
        }
    }

    if (empty($toRun)) {
        break;
    } else {
        sort($toRun);
        reset($toRun);

        $r = current($toRun);
        $steps[$r]['done'] = true;
        $runned[] = $r;
    }

} while (true);

var_dump(implode('', $runned));
