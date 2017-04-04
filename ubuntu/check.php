#!/usr/bin/php -q
<?php
$db = new PDO('sqlite:' . __DIR__ . '/desk.db');

$currentResult = $db->query('select * from desk order by id desc limit 1;')->fetch();
$currentlyStanding = ("1" == $currentResult['atdesk'] && "1" == $currentResult['standing']);
$currentlySitting = ("1" == $currentResult['atdesk'] && "0" == $currentResult['standing']);

$todaysActivity = $db->query('select * from desk where (created_at > "' . date("Y-m-d") . '") order by id desc;')->fetchAll();

foreach ($todaysActivity as $activity) {
	$standing = ("1" == $activity['atdesk'] && "1" == $activity['standing']);
	$sitting = ("1" == $activity['atdesk'] && "0" == $activity['standing']);

	if ("0" == $activity['atdesk'] || ($currentlyStanding && $sitting) || ($currentlySitting && $standing)) {
		break;
	}

	$sessionStart = new DateTime($activity['created_at']);
	$sessionLengthMinutes = (new DateTime())->diff($sessionStart)->i;
}

// 3:1 stand:sit -- https://mobilitymgmt.com/articles/2015/02/03/study-sit-to-stand-ratio.aspx?admgarea=research

if ($currentlyStanding) {
	echo "You have been standing for " . $sessionLengthMinutes . " minutes.\n";

	if ($sessionLengthMinutes >= 90) {
		$alert = "You have been standing for " . $sessionLengthMinutes . " minutes. Sit down for a bit.\n";
		exec('/usr/bin/notify-send -i error "' . $alert . '"');
	}
} elseif ($currentlySitting) {
	echo "You have been sitting for " . $sessionLengthMinutes . " minutes.\n";
	if ($sessionLengthMinutes >= 30) {
		$alert = "You have been sitting for " . $sessionLengthMinutes . " minutes. Time to stand up!\n";
		exec('/usr/bin/notify-send -i error "' . $alert . '"');
	}
} else {
	echo "You are not at your standing desk.\n";
}

// echo "Since: " . $sessionStart->format("Y-m-d H:i:s") . " (" . $sessionLengthMinutes . " minutes)\n";