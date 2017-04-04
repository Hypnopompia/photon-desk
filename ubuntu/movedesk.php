#!/usr/bin/php -q
<?php
	$photonName = "Desk";
	$presets = [
		'max'   => ['height' => 120, 'icon' => '~/bin/desk-stand-icon.png'],
		'stand' => ['height' =>  94, 'icon' => '~/bin/desk-stand-icon.png'],
		'sit'   => ['height' =>  66, 'icon' => '~/bin/desk-sit-icon.png'],
		'min'   => ['height' =>  54, 'icon' => '~/bin/desk-sit-icon.png'],
	];

	// Get today's stats
	$db = new PDO('sqlite:' . __DIR__ . '/desk.db');
	$sittingResult  = $db->query('select strftime("%Y-%m-%d", created_at) ctime, count(*) ct from desk where (atdesk=1 and standing=0 and ctime=date("now", "localtime") ) group by ctime;')->fetch();
	$sittingTime = $sittingResult['ct'];
	$standingResult = $db->query('select strftime("%Y-%m-%d", created_at) ctime, count(*) ct from desk where (atdesk=1 and standing=1 and ctime=date("now", "localtime") ) group by ctime;')->fetch();
	$standingTime = $standingResult['ct'];
	$totalTime = $standingTime + $sittingTime;

	function heightSort($a, $b) {
		if ($a['height'] == $b['height']) {
			return 0;
		}
		return ($a['height'] > $b['height']) ? -1 : 1; // Highest to lowest
	}
	uasort($presets, 'heightSort');

	function getClosest($search, $arr) {
		$closest = null;
		foreach ($arr as $k => $item) {
			if ($closest === null || abs($search - $closest) > abs($item['height'] - $search)) {
				$closest = $item['height'];
				$closestKey = $k;
			}
		}
		return $closestKey;
	}

	function getPrevious($search, $arr) {
		reset($arr);
		while(key($arr) !== $search) next($arr);
		$previous = prev($arr);
		return key($arr);
	}

	function getNext($search, $arr) {
		reset($arr);
		while(key($arr) !== $search) next($arr);
		$previous = next($arr);
		return key($arr);
	}

	function convertToHoursMins($time, $format = '%02d:%02d') {
		if ($time < 1) {
			return;
		}
		$hours = floor($time / 60);
		$minutes = ($time % 60);
		return sprintf($format, $hours, $minutes);
	}

	if (isset($argv[1])) {
		$cmd = $argv[1];

		if (is_numeric($cmd)) { // probably a height. move to it
			exec('notify-send "Moving to ' . $cmd . 'cm"');
			exec("particle call $photonName setheight " . $cmd);
		} elseif ($cmd == "up" && isset($presets[$previousPosition]) ) { // Go up one position
			exec('notify-send -i ' . $presets[$previousPosition]['icon'] . ' "Moving desk to ' . $previousPosition . '"');
			exec("particle call $photonName setheight " . $presets[$previousPosition]['height']);
		} elseif ($cmd == "down" && isset($presets[$nextPosition]) ) { // Go down one position
			exec('notify-send -i ' . $presets[$nextPosition]['icon'] . ' "Moving desk to ' . $nextPosition . '"');
			exec("particle call $photonName setheight " . $presets[$nextPosition]['height']);
		} elseif (isset($presets[$cmd])) { // go to the preset
			exec('notify-send -i ' . $presets[$cmd]['icon'] . ' "Moving desk to ' . $cmd . '"');
			exec("particle call $photonName setheight " . $presets[$cmd]['height']);
		}

	} else {
		if ($standingTime > 0 && $totalTime > 0) {
			echo "    Time Standing: " . convertToHoursMins($standingTime) . " - " . number_format($standingTime / $totalTime * 100, 1) . "%\n";
		}

		if ($sittingTime > 0 && $totalTime > 0) {
			echo "     Time Sitting: " . convertToHoursMins($sittingTime) . " - " . number_format($sittingTime / $totalTime * 100, 1) . "%\n";
		}

		echo "     Time at desk: " . convertToHoursMins($totalTime) . "\n\n";

		$currentHeight = exec("particle call $photonName getheight");
		echo "   Current Height: " . $currentHeight . "cm\n";

		$currentPosition = getClosest($currentHeight, $presets);
		$previousPosition = getPrevious($currentPosition, $presets);
		$nextPosition = getNext($currentPosition, $presets);

		if (!is_null($previousPosition)) {
			echo "      Up Position: " . $previousPosition . " (" . $presets[$previousPosition]['height'] . "cm)\n";
		}

		echo " Current Position: " . $currentPosition . " (" . $presets[$currentPosition]['height'] . "cm)\n";

		if (!is_null($nextPosition)) {
			echo "    Down Position: " . $nextPosition . " (" . $presets[$nextPosition]['height'] . "cm)\n";
		}

	}
