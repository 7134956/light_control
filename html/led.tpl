<html><head><title>Regulator</title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
<div id="main">
<center>
<h1>lamp</h1>
<p>
lamp %ledstate%. Power %ledvalue%.
</p>
<form method="post" action="led.cgi">
<input type="submit" name="led" value="0">
<input name="led" type="range" min="0" max="1023" step="1" value="%ledvalue%" onchange="javascript: f_send(this.form);">
<script type="text/javascript">
function f_send(_form) {
_form.submit();
}
</script>

</form>
</center>
</div>
</body></html>
