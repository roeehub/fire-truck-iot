import 'package:fire_truck_iot/welcome_screen.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:font_awesome_flutter/font_awesome_flutter.dart';
import 'package:google_fonts/google_fonts.dart';

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(scaffoldBackgroundColor: const Color(0xE39621E5)),
      home: MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key? key, required this.title}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;
  // final FirebaseAuth firebaseAuth = FirebaseAuth.instance;
  // final GoogleSignIn googleSignIn = GoogleSignIn();

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {

  get logoGreen => const Color(0x189C1DFF);

  @override
  Widget build(BuildContext context) {
    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.
    return Scaffold(
      // appBar: AppBar(
      //   // Here we take the value from the MyHomePage object that was created by
      //   // the App.build method, and use it to set our appbar title.
      //   title: Text(widget.title),
      // ),
      body: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          //We take the image from the assets
          Image.asset(
            'assets/images/fire_truck_no_background_img.png',
            height: 250,
          ),
          const SizedBox(
            height: 20,
          ),
          //Texts and Styling of them
          const Text(
            'Welcome to TGD !',
            textAlign: TextAlign.center,
            style: TextStyle(color: Colors.white, fontSize: 28),
          ),
          const SizedBox(height: 20),
          const SizedBox(
            width: 320,
            child: Text(
              'A one-stop portal for you to learn the latest technologies from SCRATCH',
              textAlign: TextAlign.center,
              style: TextStyle(color: Colors.white, fontSize: 16),
            ),
          ),
          const SizedBox(
            height: 30,
          ),
          //Our MaterialButton which when pressed will take us to a new screen named as
          //LoginScreen
          MaterialButton(
            elevation: 0,
            height: 50,
            onPressed: () {
              Navigator.push(context,
                  MaterialPageRoute(builder: (_) => LoginScreen()));
            },
            color: logoGreen,
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: const <Widget>[
                Text('Get Started',
                    style: TextStyle(color: Colors.white, fontSize: 20)),
                Icon(Icons.arrow_forward_ios)
              ],
            ),
            textColor: Colors.white,
          )
        ],
      ),
    );
  }
}

class LoginScreen extends StatefulWidget {

  const LoginScreen({Key? key}) : super(key: key);

  @override
  State<LoginScreen> createState() => _LoginScreenState();
}

class _LoginScreenState extends State<LoginScreen> {
  final Color primaryColor = const Color(0xff18203d);

  final Color secondaryColor = const Color(0xff232c51);

  final Color logoGreen = const Color(0xff25bcbb);

  final TextEditingController nameController = TextEditingController();

  final TextEditingController passwordController = TextEditingController();

  String email = 'Uninitialized email';

  String password = 'Uninitialized password';

  final FirebaseAuth firebaseAuth = FirebaseAuth.instance;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          backgroundColor: Colors.transparent,
          elevation: 0,
        ),
        backgroundColor: primaryColor,
        body: Container(
          alignment: Alignment.topCenter,
          margin: const EdgeInsets.symmetric(horizontal: 30),
          child: SingleChildScrollView(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              children: <Widget>[
                Text(
                  'Sign in to TGD and continue',
                  textAlign: TextAlign.center,
                  style:
                  GoogleFonts.openSans(color: Colors.white, fontSize: 28),
                ),
                const SizedBox(height: 20),
                Text(
                  'Enter your email and password below to continue to the The Growing Developer and let the learning begin!',
                  textAlign: TextAlign.center,
                  style:
                  GoogleFonts.openSans(color: Colors.white, fontSize: 14),
                ),
                const SizedBox(
                  height: 50,
                ),
                _buildTextField(
                    nameController, Icons.account_circle, 'Username'),
                const SizedBox(height: 20),
                _buildTextField(passwordController, Icons.lock, 'Password'),
                const SizedBox(height: 30),
                MaterialButton(
                  elevation: 0,
                  minWidth: double.maxFinite,
                  height: 50,
                  onPressed: () {_login();},
                  color: logoGreen,
                  child: const Text('Login',
                      style: TextStyle(color: Colors.white, fontSize: 16)),
                  textColor: Colors.white,
                ),
                const SizedBox(height: 20),
                MaterialButton(
                  elevation: 0,
                  minWidth: double.maxFinite,
                  height: 50,
                  onPressed: () {_signUp();},
                  color: Colors.blue,
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: const <Widget>[
                      Icon(FontAwesomeIcons.google),
                      SizedBox(width: 10),
                      Text('Sign-in using Google',
                          style: TextStyle(color: Colors.white, fontSize: 16)),
                    ],
                  ),
                  textColor: Colors.white,
                ),
              ],
            ),
          ),
        ));
  }

  _buildTextField(
      TextEditingController controller, IconData icon, String labelText) {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 10, vertical: 5),
      decoration: BoxDecoration(
          color: secondaryColor, border: Border.all(color: Colors.blue)),
      child: TextField(
        onChanged: (value) {
          setState(() {
            if (labelText == "Username"){
              email = value.trim();
            } else {
              password = value.trim();
            }
          });
        },
        controller: controller,
        style: const TextStyle(color: Colors.white),
        decoration: InputDecoration(
            contentPadding: const EdgeInsets.symmetric(horizontal: 10),
            labelText: labelText,
            labelStyle: const TextStyle(color: Colors.white),
            icon: Icon(
              icon,
              color: Colors.white,
            ),
            // prefix: Icon(icon),
            border: InputBorder.none),
      ),
    );
  }

  _login(){
    signIn(email, password);
  }

  _signUp(){
    debugPrint(password);
    firebaseAuth.createUserWithEmailAndPassword(email: email, password: password)
        .then((value) => {
          debugPrint("------------------"),
          debugPrint(value.toString()),
          debugPrint("------------------"),
        });
    Navigator.push(
      context,
      MaterialPageRoute(builder: (context) => const WelcomeScreen()),
    );
  }

  void signIn(String email, String password) async {
    String errorMessage = '';

    try {
      UserCredential result = await firebaseAuth.signInWithEmailAndPassword(email: email, password: password);
      debugPrint(result.user.toString());
    } on FirebaseAuthException catch (error) {
      switch (error.code) {
        case "ERROR_EMAIL_ALREADY_IN_USE":
        case "account-exists-with-different-credential":
        case "email-already-in-use":
          errorMessage = "Email already used. Go to login page.";
          break;
        case "ERROR_WRONG_PASSWORD":
        case "wrong-password":
          errorMessage = "Wrong email/password combination.";
          break;
        case "ERROR_USER_NOT_FOUND":
        case "user-not-found":
          errorMessage = "No user found with this email.";
          break;
        case "ERROR_USER_DISABLED":
        case "user-disabled":
          errorMessage = "User disabled.";
          break;
        case "ERROR_TOO_MANY_REQUESTS":
        case "operation-not-allowed":
          errorMessage = "Too many requests to log into this account.";
          break;
        case "ERROR_OPERATION_NOT_ALLOWED":
          errorMessage = "Server error, please try again later.";
          break;
        case "ERROR_INVALID_EMAIL":
        case "invalid-email":
          errorMessage = "Email address is invalid.";
          break;
        default:
          errorMessage = "Login failed. Please try again.";
          break;
      }
    }

    if (errorMessage != '') {
      // return Future.error(errorMessage);
      debugPrint(errorMessage);
      errorMessage = '';
    } else {
      Navigator.push(
        context,
        MaterialPageRoute(builder: (context) => const WelcomeScreen()),
      );
    }

  }

}

