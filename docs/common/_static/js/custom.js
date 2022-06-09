var preUrl = ""
var hostRoot = ""
var sdkList = [];


var BK7235 =
{
	name: "bk7235",
	lang: [ "zh-cn" ],
	version: [
		"latest"
	]
}

var BK7237 =
{
	name: "bk7237",
	lang: [ "zh-cn" ],
	version: [
		"latest"
	]
}

var BK7256 =
{
	name: "bk7256",
	lang: [ "zh-cn" ],
	version: [
		"latest"
	]
}


sdkList.push(BK7235);
sdkList.push(BK7237);
sdkList.push(BK7256);

jQuery(function()
{
    var origin = window.location.origin;
 
	console.log(origin);
	buildMultiVersionSelector();

    setPageStyle();
});


function buildMultiVersionSelector()
{
 
    var curUrl = window.location.pathname;
 
	console.log("Current URL: " + curUrl)

	var urlList = curUrl.split('/');
 
	if (urlList.length <= 3)
	{
        console.log("url error: " + curUrl);
        return;
    }

    var target = urlList[1];
    var language = urlList[2] || "zh-cn";
    var version = urlList[3];
	var index;

	for (index = 0; index < urlList.length; index++)
	{
		if (urlList[index] == "zh_CN" || urlList[index] == "en")
		{
			language = urlList[index];
			target = urlList[index - 1];
			version = urlList[index + 1];
			break;
		}
	}

	if (urlList.length > 3)
	{
		preUrl = ""

		for (var i = 1; i < index - 1; i++)
		{
			preUrl += urlList[i] + "/"
		}
	}



	hostRoot = "/" + preUrl + target + "/" + language + "/" + version

	console.log("Index: " + index + " Name: " + target + " Language: " + language + " Version: " + version)

	console.log("Url: " + hostRoot)

    var searchUrl = $("#rtd-search-form").attr('action');

	var versionPage = `
		<div role="search">
            <div class="target_selectors"	style="margin-top: 5px; display: flex;">
		        <select id="target-id" style="width: 55%; margin-top: 5px; border-radius: 5px;"></select>
                <span style="color: black; display: flex; align-items: center;">&nbsp;-&nbsp;</span>
                <select id="version-id" style="width: 60%; margin-top: 5px; border-radius: 5px;"></select>
            </div>
                <form id="rtd-search-form" class="wy-form" action="search.html" method="get" style="margin-top: 5px;">
		            <input type="text" name="q" placeholder="Search docs" style="border-radius: 5px; margin-top: 10px; border-color: #CCC; height: 35px;">
                    <input type="hidden" name="check_keywords" value="yes">
                    <input type="hidden" name="area" value="default">
                </form>
		</div>
		`;


    $("[role=search]").html(versionPage);
    $("#rtd-search-form").attr('action', searchUrl);
	$("#target-id").html("");
	$("#version-id").html("");

	$("#target-id").append("<option value disabled selected>Target...</option>");
	$("#version-id").append("<option value disabled selected>Version...</option>");

	for(var i = 0; i < sdkList.length; i++)
	{
		$("#target-id").append("<option value='" + sdkList[i].name + "'>" + sdkList[i].name + "</option>");

		if(sdkList[i].name == target)
		{
			var list = sdkList[i].version;

			for(var j = 0; j < list.length; j++)
			{
				$("#version-id").append("<option value='" + list[j] + "'>"+ list[j] + "</option>");
			}
		}
	}

	$("#target-id").val(target);
	$("#version-id").val(version);

	$("#target-id").bind('change', function()
	{
        var target = $("#target-id").val();

        for(var i = 0; i < sdkList.length; i++)
		{
            if(sdkList[i].name == target)
			{
                var list = sdkList[i].version;

                for(var j = 0; j < list.length; j++)
				{
                    var url = "/" + preUrl + target + "/" + language + "/" + list[j] + "/index.html";
                    window.location.href = url;
			    }
			}
        }
	});

	$("#version-id").bind('change', function()
	{
        var target = $("#target-id").val();
        var version = $("#version-id").val();

        if(version == "")
		{
			return;
		}

        var url = "/" + preUrl + target + "/" + language + "/" + version + "/index.html";
        window.location.href = url;

    });
}


function setPageStyle()
{

    console.log($(".wy-breadcrumbs-aside").html())

    var stylePage = `
        <img id="styleIcon" src="_static/light.png" style="width: 24px; height: 24px; display: inline; align-items: center; cursor: pointer; margin-left:5px"></img>
        <img id="modeIcon" src="_static/open.png" style="width: 24px; height: 24px; display: inline; align-items: center; cursor: pointer; margin-left:5px"></img>
    `;
    $(".wy-breadcrumbs-aside").append(stylePage);

    var mode = localStorage.getItem('mode') || "light";
    setPageMode(mode);

    $("#styleIcon").bind('click', function()
	{

       var mode = $("#styleIcon").attr('mode'); 

       if(mode == "light")
	   {
           mode = "dark";
       }
	   else
	   {
           mode = "light";
       }

       setPageMode(mode);

    });
 
	var styleSet = localStorage.getItem('styleSet') || "false";
 
	setPageWidth(styleSet);

    $("#modeIcon").bind('click', function()
	{

       var styleSet = $("#modeIcon").attr('styleSet'); 

       if(styleSet == "false")
	   {
           styleSet = "true";
       }
	   else
	   {
           styleSet = "false";
       }
       setPageWidth(styleSet);
    });
}


function setPageMode(mode)
{

    if(mode == "light")
	{
        $("<link>").attr({ rel: "stylesheet", type: "text/css", href: hostRoot + "/_static/css/light.css" }).appendTo("head");
        $("#styleIcon").attr('src', hostRoot + '/_static/dark.png')
        $("#styleIcon").attr('mode', 'light');
        localStorage.setItem('mode', 'light');
    }
	else
	{
        $("<link>").attr({ rel: "stylesheet", type: "text/css", href: hostRoot + "/_static/css/dark.css" }).appendTo("head");
        $("#styleIcon").attr('src', hostRoot + '/_static/light.png')
        $("#styleIcon").attr('mode', 'dark');
        localStorage.setItem('mode', 'dark');
    }
}


function setPageWidth(styleSet)
{
 
	if(styleSet == "false")
	{
        $("#modeIcon").attr('src', hostRoot + '/_static/open.png')
        $("#modeIcon").attr('styleSet', 'false');
        $(".wy-nav-content").attr('style', 'max-width:888px');
        localStorage.setItem('styleSet', 'false');
    }
	else
	{
        $("#modeIcon").attr('src', hostRoot + '/_static/close.png')
        $("#modeIcon").attr('styleSet', 'true');
        $(".wy-nav-content").attr('style', 'max-width:none');
        localStorage.setItem('styleSet', 'true');
    }
}
