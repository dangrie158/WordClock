// Ionic Starter App

// angular.module is a global place for creating, registering and retrieving
// Angular modules
// 'starter' is the name of this angular module example (also set in a <body>
// attribute in index.html)
// the 2nd parameter is an array of 'requires'
// 'starter.services' is found in services.js
// 'starter.controllers' is found in controllers.js
angular
    .module('app', [
              'ionic',
              'app.controllers',
              'app.routes',
              'app.services',
            ])

.run(function($ionicPlatform) {
    $ionicPlatform.ready(function() {
        // Hide the accessory bar by default (remove this to show the accessory
        // bar above the keyboard
        // for form inputs)
        if (window.cordova && window.cordova.plugins &&
            window.cordova.plugins.Keyboard) {
            cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
            cordova.plugins.Keyboard.disableScroll(true);
        }
        if (window.StatusBar) {
            // org.apache.cordova.statusbar required
            StatusBar.styleDefault();
        }
    });
});

angular.module('app.routes', [])
    .config(function($stateProvider, $urlRouterProvider) {

        // Ionic uses AngularUI Router which uses the concept of states
        // Learn more here: https://github.com/angular-ui/ui-router
        // Set up the various states which the app can be in.
        // Each state's controller can be found in controllers.js
        $stateProvider.state('wordClockSetup', {
            url: '/page1',
            templateUrl: 'wordClockSetup.html',
            controller: 'wordClockSetupCtrl'
        });

        $urlRouterProvider.otherwise('/page1');
    });

angular.module('app.controllers', []).controller('wordClockSetupCtrl', [
  '$scope', '$stateParams', 'RemoteService', // The following is the constructor function for
                            // this page's controller. See
                            // https://docs.angularjs.org/guide/controller
  // You can include any angular dependencies as parameters for this function
  // TIP: Access Route Parameters for your page via $stateParams.parameterName
  function($scope, $stateParams, RemoteService) {
        $scope.options = {};

        $scope.update = function() {
            if ($scope.options.usentp) {
                var ntpServer = $scope.options.server;
                RemoteService.updateNTPServer(ntpServer);
            }
            else {
                var time = $scope.options.time;
                RemoteService.updateTime(time);
            }
        };

        $scope.connect = function() {
            $scope.connecting = true;
            var station = $scope.options.station;
            var passphrase = $scope.options.passphrase;

            RemoteService.updateWiFi(station, passphrase).
            then(function(response) {
                $scope.connecting = false;
                $scope.station = response.data;
            }).catch(function() {
                $scope.connecting = false;
                $scope.options.station = null;
            });
        };

        $scope.updateBrightness = function() {
            var brightness = $scope.options.brightness;
            RemoteService.updateBrightness(brightness);
        };

        $scope.updateTimezone = function(){
            var timezone = $scope.options.timezone;
            RemoteService.updateTimezone(timezone);
        }

        $scope.scan = function() {
            $scope.stations = [];
            RemoteService.searchStations()
                .then(function(response) {
                    $scope.stations = response.data;
                });
        };

        $scope.getOptions = function() {
            RemoteService.getTime()
                .then(function(response) {
                    $scope.options.time = response.data;
                });

            RemoteService.getNTPServer()
                .then(function(response) {
                    $scope.options.server = response.data;
                });

            RemoteService.getStation()
                .then(function(response) {
                    $scope.options.station = response.data;
                });

            RemoteService.getBrightness()
                .then(function(response) {
                    $scope.options.brightness = response.data;
                });

                RemoteService.getTimezone()
                    .then(function(response) {
                        $scope.options.timezone = response.data;
                    });
        };

        $scope.scan();
        $scope.getOptions();
}]);

angular.module('app.services', [])
    .service('RemoteService', ['$http', '$httpParamSerializerJQLike', function($http, $httpParamSerializerJQLike) {

        this.postForm = function(url, data) {
            return $http({
                method: 'POST',
                url: url,
                data: $httpParamSerializerJQLike(data),
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                }
            });
        };

        this.getTime = function() {
            return $http.get('/time');
        };

        this.updateTime = function(newTime) {
            return this.postForm('/time', {
                time: newTime
            });
        };

        this.getBrightness = function() {
            return $http.get('/brightness');
        };

        this.updateBrightness = function(newBright) {
            return this.postForm('/brightness', {
                brightness: newBright
            });
        };

        this.getTimezone = function() {
            return $http.get('/timezone');
        };

        this.updateTimezone = function(newTz) {
            return this.postForm('/timezone', {
                timezone: newTz
            });
        };

        this.searchStations = function() {
            return $http.get('/stations');
        };

        this.getStation = function() {
            return $http.get('/station');
        };

        this.updateWiFi = function(newStation, newPassphrase) {
            return this.postForm('/station', {
                station: newStation,
                passphrase: newPassphrase
            });
        };

        this.getNTPServer = function() {
            return $http.get('/server');
        };

        this.updateNTPServer = function(newServer) {
            return this.postForm('/server', {
                server: newServer
            });
        };

    }]);
