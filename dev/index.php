<?php
# Display this directory to users
# (code used to display test results)

$dir = glob("zipios*");

echo "<html><head><title>Zipios++ coverage, statistics, and test log information</title></head>";
echo "<body><h1>Zipios++ coverage</h1>";
echo "<p><a href=\"documentation\">Zipios++ Doxygen Documention</a><p>";
echo "<table border=\"1\" cellpadding=\"10\" cellspacing=\"0\"><tbody><tr><th>Coverage</th><th>Statistics</th><th>Test Logs</th></tr>";
foreach($dir as $d)
{
	echo "<tr>";

	echo "<td><a href=\"", $d, "\">", $d, "</a></td>";
	echo "<td><a href=\"", $d, "/statistics.html\">", $d, "/statistics.html</td>";
	echo "<td><a href=\"", $d, "/test_log.html\">", $d, "/test_log.html</td>";

        echo "</tr>";
}
echo "</tbody></table></body></html>";
