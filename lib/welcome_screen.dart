import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:external_app_launcher/external_app_launcher.dart';

// class MyAppTest extends StatelessWidget {
//   const MyAppTest({Key? key}) : super(key: key);
//
//   // This widget is the root of your application.
//   @override
//   Widget build(BuildContext context) {
//     return MaterialApp(
//       title: 'Flutter Demo',
//       theme: ThemeData(scaffoldBackgroundColor: const Color.fromRGBO(230, 138, 0, 1)),
//       home: const WelcomeScreen(),
//     );
//   }
// }

class WelcomeScreen extends StatefulWidget {
  const WelcomeScreen({Key? key}) : super(key: key);

  @override
  WelcomeScreenState createState() => WelcomeScreenState();
}

class WelcomeScreenState extends State<WelcomeScreen> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: Column(children: [
          const SizedBox(height: 200),
          const Text(
            'More description of the game?\nCredits?',
            textAlign: TextAlign.center,
            style: TextStyle(color: Colors.white, fontSize: 28),
          ),
          const SizedBox(height: 250),
          ElevatedButton(
            // onPressed: () async {
            //   Future.delayed(const Duration(milliseconds: 100), () async {
            //     await LaunchApp.openApp(
            //         androidPackageName: 'com.shevauto.remotexy.free',
            //         openStore: false
            //     );
            //   });
            // },
            onPressed: () {
              FirebaseFirestore.instance
                  .collection('data')
                  .add({'text': 'data added through app'});
            },
            style: ButtonStyle(
              shape: MaterialStateProperty.all(RoundedRectangleBorder(borderRadius: BorderRadius.circular(20))),
              padding: MaterialStateProperty.all(const EdgeInsets.all(30)),
              backgroundColor: MaterialStateProperty.all(Colors.blue), // <-- Button color
              overlayColor: MaterialStateProperty.resolveWith<Color?>((states) {
                if (states.contains(MaterialState.pressed)) return Colors.red; // <-- Splash color
              }),
            ), child: const Text("GO!"),
          )
        ],),
      ),
    );
  }
}
