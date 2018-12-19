<?php

$fileName = __DIR__ . '/7.txt';
$workersCount = 5;
$readyTime = 60;

$lines = file($fileName);

$steps = [];
$stepEntity = [
    'done' => false,
    'running' => false,
    'ready_at' => -1,
    'worker' => -1,
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

$now = 0;
$running = [];
$runned = [];
$workers = [];

do {
    $toRun = [];

    foreach ($steps as $step => $config) {
        if (!$config['done'] && !$config['running']) {
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

    foreach ($toRun as $r) {
        for ($i = 0; $i < $workersCount; $i++) {
            if (empty($workers[$i])) {
                $workers[$i] = true;
                $steps[$r]['running'] = true;
                $steps[$r]['worker'] = $i;
                $steps[$r]['ready_at'] = $now + $readyTime + ord($r) - 65;
                $running[$r] = true;
                break;
            }
        }
    }

    if (empty($running)) {
        break;
    }

    foreach ($running as $r => $true) {
        if ($now === $steps[$r]['ready_at']) {
            unset($running[$r]);
            $steps[$r]['done'] = true;
            $steps[$r]['running'] = false;
            $workers[$steps[$r]['worker']] = false;
            $runned[] = $r;
        }
    }

    $now++;

} while (true);

var_dump(implode('', $runned));
var_dump($now);
