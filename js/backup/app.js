$(document).ready(function() {

    //////////////////////////////////////
    //          SNIPPETS                //
    //////////////////////////////////////
    $(function() {
      $('a[href*=#]:not([href=#])').click(function() {

        if (location.pathname.replace(/^\//,'') == this.pathname.replace(/^\//,'') && location.hostname == this.hostname) {
          var target = $(this.hash);
          if (target.selector.includes("carousel")) return; //carousel hack fix
          target = target.length ? target : $('[name=' + this.hash.slice(1) +']');
          if (target.length) {
            $('html,body').animate({
              scrollTop: target.offset().top
            }, 1000);
            return false;
          }
        }
      });
    });

    /////////////////////////////////////
    //      MODAL WINDOW GALLERY       //
    /////////////////////////////////////
    $('.gallery-item').click(function() {
        var label = $('#myModalLabel');
        var img = $('#showcase-img');
        var price = $('#modal-price-tag');
        var body = $('#modalBody');
        var dataModel = $(this).find('img');
        var modelfeats = $('#model-feats').find('li');

        label.text('Dragonic: ' + $(this).find('.item-name-caption').text());
        price.text($(this).find('.item-price-caption').text());
        img.attr('src', dataModel.attr('src'));
    });

    $('div.thumbnail-50').on('click', function(){
        $('img.img-thumbnail').addClass('thumb-50');
        $('ul#model-feats').addClass('hide');
    });

    $('article.thumbnail-100').on('click', function(){
        $('img.img-thumbnail').removeClass('thumb-50');
        $('ul#model-feats').removeClass('hide');
    });

    /////////////////////////////////////
    //      MODAL WINDOW FEATURES      //
    /////////////////////////////////////
    $('.features-item').click(function() {
        var label = $('#myModalLabel');
        var img = $('#showcase-img');
        var price = $('#modal-price-tag');
        var body = $('#modalBody');
        var dataModel = $(this).find('img');
        var modelfeats = $('#model-feats').find('li');

        label.text('Hero Name: ' + $(this).find('.item-name-caption').text());
        price.text($(this).find('.item-price-caption').text());
        img.attr('src', dataModel.attr('src'));


        $(modelfeats[0]).text('Strength: ' + dataModel.data('strength')); //Strength
        $(modelfeats[1]).text('Dexterity: ' + dataModel.data('dexterity')); //Dexterity
        $(modelfeats[2]).text('Agility: ' + dataModel.data('agility')); //Agility
        $(modelfeats[3]).text('Willpower: ' + dataModel.data('willpower')); //Willpower
        $(modelfeats[4]).text('Intelligence: ' + dataModel.data('intelligence')); //Intelligence
    });

    $(document).ready(function(){
    $('.shop-modal-hide').click(function(){
        $('#shop-modal').modal('hide');
        }); 
    });

    /////////////////////////////////////
    //          GOOGLE MAPS            //
    /////////////////////////////////////
    var map = new GMaps({
        div: '#gmaps',
        lat: 37.9838096,
        lng: 23.7275388,
        zoom: 14
    });


    var styles = [{"featureType":"administrative","elementType":"labels.text.fill","stylers":[{"color":"#ffcc88"}]},{"featureType":"administrative","elementType":"labels.text.stroke","stylers":[{"color":"#331111"}]},{"featureType":"administrative.country","elementType":"geometry","stylers":[{"visibility":"on"}]},{"featureType":"administrative.province","elementType":"geometry.stroke","stylers":[{"visibility":"off"}]},{"featureType":"administrative.province","elementType":"labels.text","stylers":[{"weight":"0.01"},{"saturation":"0"},{"color":"#ffcc88"},{"visibility":"simplified"}]},{"featureType":"administrative.locality","elementType":"labels.text","stylers":[{"visibility":"on"}]},{"featureType":"administrative.locality","elementType":"labels.text.stroke","stylers":[{"visibility":"on"},{"weight":"2.00"}]},{"featureType":"administrative.neighborhood","elementType":"labels.text","stylers":[{"visibility":"on"}]},{"featureType":"administrative.land_parcel","elementType":"labels.text","stylers":[{"visibility":"off"}]},{"featureType":"landscape.man_made","elementType":"all","stylers":[{"color":"#331111"},{"gamma":1.57}]},{"featureType":"landscape.man_made","elementType":"labels.text","stylers":[{"visibility":"on"}]},{"featureType":"landscape.man_made","elementType":"labels.text.fill","stylers":[{"visibility":"on"}]},{"featureType":"landscape.natural","elementType":"all","stylers":[{"color":"#331111"},{"gamma":1.39}]},{"featureType":"poi","elementType":"geometry","stylers":[{"color":"#331111"},{"gamma":1.22}]},{"featureType":"poi","elementType":"labels.text.fill","stylers":[{"color":"#ffcc88"},{"visibility":"on"}]},{"featureType":"poi","elementType":"labels.text.stroke","stylers":[{"color":"#331111"},{"weight":2.6}]},{"featureType":"poi","elementType":"labels.icon","stylers":[{"visibility":"simplified"},{"lightness":-100},{"gamma":1},{"saturation":-72},{"weight":0.1}]},{"featureType":"road","elementType":"geometry","stylers":[{"color":"#331111"}]},{"featureType":"road","elementType":"labels.text.fill","stylers":[{"color":"#ffcc88"}]},{"featureType":"road","elementType":"labels.text.stroke","stylers":[{"color":"#331111"}]},{"featureType":"transit","elementType":"geometry","stylers":[{"color":"#331111"}]},{"featureType":"transit.station.airport","elementType":"labels.text.fill","stylers":[{"color":"#331111"},{"gamma":"2"}]},{"featureType":"transit.station.airport","elementType":"labels.text.stroke","stylers":[{"color":"#331111"}]},{"featureType":"transit.station.airport","elementType":"labels.icon","stylers":[{"visibility":"on"}]},{"featureType":"water","elementType":"all","stylers":[{"color":"#331111"}]},{"featureType":"water","elementType":"labels.text.fill","stylers":[{"color":"#331111"},{"gamma":"1.50"}]},{"featureType":"water","elementType":"labels.text.stroke","stylers":[{"weight":"2"}]}];

    map.setOptions({styles: styles});

    map.addMarker({
        lat: 37.9838096,
        lng: 23.7275388,
        title: 'Athens HQ',
        infoWindow: {
            content: '<p>Our headquarters and main workshop situated in beautiful Athens &hearts; Come see us!</p>'
        }
    });

    /////////////////////////////////////
    //          ANIMATIONS             //
    /////////////////////////////////////
    var onMobile = $(window).width() < 768;

    $('.features').waypoint(function(dir) {
        $('#features .to-fade-in').addClass('animated fadeIn');
    }, { offset: '60%' });

    $('.testimonials').waypoint(function(dir) {
        $('#testimonials .to-fade-in').addClass('animated fadeIn');
    }, { offset: '60%' });

    $('.store').waypoint(function(dir) {
        $('.store-btn').addClass('animated flipInY');
    }, { offset: '40%' });

    $('.contact').waypoint(function(dir) {
        $('#gmaps').addClass('animated fadeInRight');
    }, { offset: '50%' });

});

    /* Light YouTube Embeds by @labnol */
    /* Web: http://labnol.org/?p=27941 */

    document.addEventListener("DOMContentLoaded",
        function() {
            var div, n,
                v = document.getElementsByClassName("youtube-player");
            for (n = 0; n < v.length; n++) {
                div = document.createElement("div");
                div.setAttribute("data-id", v[n].dataset.id);
                div.innerHTML = labnolThumb(v[n].dataset.id);
                div.onclick = labnolIframe;
                v[n].appendChild(div);
            }
        });

    function labnolThumb(id) {
        var thumb = '<img src="https://i.ytimg.com/vi/ID/hqdefault.jpg">',
            play = '<div class="play"></div>';
        return thumb.replace("ID", id) + play;
    }

    function labnolIframe() {
        var iframe = document.createElement("iframe");
        var embed = "https://www.youtube.com/embed/ID?autoplay=1";
        iframe.setAttribute("src", embed.replace("ID", this.dataset.id));
        iframe.setAttribute("frameborder", "0");
        iframe.setAttribute("allowfullscreen", "1");
        this.parentNode.replaceChild(iframe, this);
    }

    /////////////////////////////////////
    //        CAROUSEL SWIPE           //
    /////////////////////////////////////   

$(document).ready(function() {
   $("#dragonic-carousel").swiperight(function() {
      $(this).carousel('prev');
    });
   $("#dragonic-carousel").swipeleft(function() {
      $(this).carousel('next');
   });
});

    /////////////////////////////////////
    //   CHANGE CLASSES RESPONSIVE     //
    /////////////////////////////////////  

$(window).resize(function(){
   console.log('resize called');
   var width = $(window).width();
   if(width >= 300 && width <= 768){
       $('.item-1').removeClass('col-sm-3').addClass('col-sm-5');
       $('.item-2').removeClass('col-sm-3').addClass('col-sm-5');
       $('.item-3').removeClass('col-sm-3').addClass('col-sm-5');
       $('.item-3').removeClass('col-sm-offset-1').addClass('col-sm-offset-3');
       $('.item-1').removeClass('col-xs-5').addClass('col-xs-12');
       $('.item-2').removeClass('col-xs-5').addClass('col-xs-12');
       $('.item-2').removeClass('col-xs-offset-2').addClass('col-xs-offset-0');       
       $('.item-3').removeClass('col-xs-5').addClass('col-xs-12');       
       $('.button-submit').removeClass('col-sm-4').addClass('col-sm-2');
   }
   else{
       $('.item-1').removeClass('col-sm-5').addClass('col-sm-3');
       $('.item-2').removeClass('col-sm-5').addClass('col-sm-3');
       $('.item-3').removeClass('col-sm-5').addClass('col-sm-3');
       $('.item-3').removeClass('col-sm-offset-3').addClass('col-sm-offset-1');
       $('.item-1').removeClass('col-xs-12').addClass('col-xs-5');
       $('.item-2').removeClass('col-xs-12').addClass('col-xs-5');
       $('.item-2').removeClass('col-xs-offset-0').addClass('col-xs-offset-2');        
       $('.item-3').removeClass('col-xs-12').addClass('col-xs-5');       
       $('.button-submit').removeClass('col-sm-2').addClass('col-sm-4');
   }
})
.resize();//trigger the resize event on page load.

    /////////////////////////////////////
    //        STOP PINCH ZOOM          //
    /////////////////////////////////////  

document.addEventListener('gesturestart', function (e) {
    e.preventDefault();
});