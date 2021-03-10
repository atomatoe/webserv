<?php
$some = $_SERVER['QUERY_STRING'];
$arr = explode ( '&' , $some , PHP_INT_MAX );
$num1 = explode ( '=' , $arr[0] , PHP_INT_MAX );
$num2 = explode ( '=' , $arr[1] , PHP_INT_MAX );
print "Status: 200\r\n\r\n";
echo '<html>';
echo '<head>';
echo '</head>';
echo '<body>';
echo '<p>', $num1[1], ' + ', $num2[1], ' = ', $num1[1] + $num2[1], '</p>';
echo '</body>';
echo '</html>';
?>
