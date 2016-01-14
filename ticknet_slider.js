if (Drupal.jsEnabled) {
    // Run the following code when the DOM has been fully loaded.
    $(document).ready(function () {

        $('input[name=reorder]').click(function () {
            var weights='';
            var events='';
            var i=0;
            while($('.first-cell').eq(i).text()!=''){
                weights+=$('select.list-weight option:selected').eq(i).text()+'%';
                events+=$('.first-cell').eq(i).text()+'%';
                i++;
            }
            var path=Drupal.settings.basePath;
            var URL=path+"reorder";
            var orderRow = function () {
                $("tr.drag-previous").removeClass("drag-previous");
                $("span.tabledrag-changed,div.warning").remove();
            }
            $.ajax({
                type: 'POST', // Use the POST method.
                url: URL,
                data: {
                    weights:weights,
                    events:events
                },
                dataType: 'json',
                success: orderRow
            });
            return false;
        });
//        alert("HO!");
        $("#block-ticknet_slider-0 .row").css('opacity','0');
        $(".bottom-background").css('opacity','0.6');        
        $("#block-ticknet_slider-0 .row").hover(
            function(){
                $(this).css('opacity','0.5')
                },
            function(){
                $(this).css('opacity','0')
                }
            );
        
        var timer=Drupal.settings.ticknet_slider.timer;
        $(".active.top-slider").css('opacity','0');
        $(".active.top-slider").eq(0).css('opacity','1').addClass('current');
        var len=$('.top-slider').length;
        if (len > 1) {
            var i = 0;
            setInterval(function(){
                topAnime(0);
                $(".active.top-slider").eq(i).fadeTo(500, 0, function(){
                    $(this).css('display','none').removeClass('current');
                    i++;
                    if (i == len)
                        i = 0;
                    topAnime(1);
                    $(".active.top-slider").eq(i).css('display','block').fadeTo(500, 1.0).addClass('current');

                });
            }, timer);
        }
        
//Работа с кнопками        
        
        var count=$(".active.top-slider").length;
        var i=0;
        $('div.right-row').click(function(){
            $(".top-slider").eq(i).fadeTo(300, 0).css("display","none");
            if(i==count-1) i=-1;
            $(".top-slider").eq(i+1).css("display","block").fadeTo(300, 1);
            i++;
        });
        $('div.left-row').click(function(){
            $(".top-slider").eq(i).fadeTo(300, 0).css("display","none");
            if(i==0) i=count;
            $(".top-slider").eq(i-1).css("display","block").fadeTo(300, 1);
            i--;
        });
        $(".stop").click(function(){
            $('.active.top-slider').removeClass('active');
            $(".stop").css('display','none');
            $(".start").css('display','block');
            $('.top-slider').removeClass('current');
        })
        $(".start").click(function(){
            $('.top-slider').addClass('active');
            $(".start").css('display','none');
            $(".stop").css('display','block');
        })
    })
}
function topAnime(type){
    if(type=='0'){
        $(".active .bottom-background").animate({
            height: "0",
            marginTop: "45px"
        }, 500 );
    } else{
        $(".active .bottom-background").animate({
            height: "45px",
            marginTop: "0"
        }, 500 );
    }
}
