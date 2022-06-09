(function() {
    /* addEventListener that also works for IE
     * Source:
     * https://stackoverflow.com/a/15564058
     */
    function addEvent(element, eventName, fn) {
        if (element.addEventListener)
            element.addEventListener(eventName, fn, false);
        else if (element.attachEvent)
            element.attachEvent('on' + eventName, fn);
    }

    addEvent(window, 'load', function(){
        document.querySelectorAll('.rst-widget-sidebar .rst-widget-header + .rst-widget-body').forEach(function(e) {
            e.previousElementSibling.onclick = function() { e.classList.toggle('shift-up'); };
        });
        document.querySelector('*[data-toggle="wy-nav-top"]').onclick = function() {
            document.querySelector('.rst-widget-sidebar').classList.toggle('shift');
        };
    });
})();