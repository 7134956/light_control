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
</form>
</div>
</body></html>
