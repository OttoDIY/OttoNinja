(function($) {
  $(function() {

 /* Settings
    =============================================== */
    // If no URL is hardcoded in the url variable then the user will be prompted
    // to enter a URL which will be stored in localStorage
    // example: http://192.168.1.50
    var url = '';

 /* Variables
    =============================================== */
    var $loader = $('.progress');
    var localStorageUrl = localStorage.getItem( 'url' );
    var $btn = $('.btn-command');
    var modelIsVisable = false;

 /* Functions
    =============================================== */
    function onInit() {
			checkForUrl();

			// listen for button clicks
			$btn.on('click', function() {
				var command = $(this).data('command');      	
				
				if (command) {
					sendCommand(command);
				}
      	
      });
    }

    function checkForUrl() {
        // if no url is found then prompt user to enter url
        if (!url) {
            if ( localStorageUrl ) {
                url = localStorageUrl;
            } else {
                promptUserForUrl();
                return; 
            }
        }
    }

    function promptUserForUrl() {
    		if (modelIsVisable) {
    			// avoid multiple modals
					return;
    		}

    		modelIsVisable = true;

        var $modal = $('#modal-prompt').modal({
            dismissible: false,
            onCloseEnd: function() {
                setUrlInLocalStorage();
                modelIsVisable = false;
            }
        }); 
        $modal.modal('open');
    }

    function setUrlInLocalStorage() {
        // update global variable and store in local storage
        url = $( '#url' ).val();
        localStorage.setItem( 'url', url );        
    }

    function sendCommand(command) {
    	console.log('GET /' + command);
      
      $loader.show();

      $.ajax({ 
          type: 'GET',
          url: url + '/' + command,
          dataType: 'json',
          timeout: 5000,
          success: function(data) {
            console.log(data);
          },
          error: function() {
              $('.status-text').html('Error: no response from device');

              // Prompt user to re-enter URL:
              setTimeout(function(){ promptUserForUrl(); }, 3000);
          },
          complete: function() {
              $loader.hide();
          }
      });
    }

    // Let's go!
    onInit();

  }); // end of document ready
})(jQuery); // end of jQuery name space
