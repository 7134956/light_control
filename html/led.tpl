<html><head><title>Регулятор</title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
<div id="main">
<h1>Лампа</h1>
<p>
Лампа %ledstate%. Яркость %ledvalue%.
</p>
<form method="post" action="led.cgi">
<input name="led" type="range" min="0" max="1023" step="1" value="%ledvalue%" onchange="javascript: f_send(this.form);">
<script type="text/javascript">
function f_send(_form) {
_form.submit();
}
</script>
<p>
<input type="submit" name="led" value="0">
<input type="submit" name="led" value="1">
<input type="submit" name="led" value="4">
<input type="submit" name="led" value="16">
<input type="submit" name="led" value="1023">
</p>
</form>
</div>
</body></html>
