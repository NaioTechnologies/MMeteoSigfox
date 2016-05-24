var time = 9;

$(window).ready(function() {
	var interval = setInterval(function() { 
		if (time > 0) { 
			$("#captchahidden").val(time);
			$("#captchasec").html(time+" s");
			time--;
		}
		else { 
			time--;
			$("#captchahidden").val(time);
			$("#captchatext").remove();
			$("#captchasubmit").removeAttr('disabled');
			$("#captchasubmit").css("display", "block");
			time--;
			$("#captchahidden").val(time);
			clearInterval(interval);
		}
	}, 1000);
});