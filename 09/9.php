<?php

gc_disable();

$players = 471;
$marbles = 72026 * 100;

$scores = [];
for ($i = 0; $i < $players; $i++) {
    $scores[$i] = 0;
}

class Marble
{
    /** @var Marble */
    public $prev;

    /** @var Marble */
    public $next;

    /** @var int */
    public $value;
}

$first = new Marble();
$first->value = 0;

$first->next = $first;
$first->prev = $first;

$crtMarble = $first;
$maxValue = 0;
$crtPlayer = 0;

do {
    $m = new Marble();
    $m->value = ++$maxValue;

    if ($m->value % 23 === 0) {
        $crtMarble = $crtMarble->prev->prev->prev->prev->prev->prev->prev;

        $scores[$crtPlayer] += $m->value;
        $scores[$crtPlayer] += $crtMarble->value;

        $crtMarble->prev->next = $crtMarble->next;
        $crtMarble->next->prev = $crtMarble->prev;

        $crtMarble = $crtMarble->next;
    } else {
        $crtMarble->next->next->prev = $m;
        $m->next = $crtMarble->next->next;
        $m->prev = $crtMarble->next;
        $m->prev->next = $m;

        $crtMarble = $m;
    }

    $crtPlayer++;
    if ($crtPlayer === $players) {
        $crtPlayer = 0;
    }
} while ($maxValue < $marbles);

var_dump(max($scores));
