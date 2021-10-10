import 'package:flutter/material.dart';

class wordSearch extends SearchDelegate<String> {
  List<String> words = [
    'abortion',
    'about',
    'above',
    'arcaea',
    'arcade',
    'biscuit',
    'because',
    'behave',
    'behalf',
    'boring',
    'bored',
  ];
  var selected;
  @override
  List<Widget> buildActions(BuildContext context) {
    return [
      IconButton(
          icon: Icon(Icons.clear),
          onPressed: () {
            query = '';
          })
    ];
  }

  @override
  Widget buildLeading(BuildContext context) {
    return IconButton(
      icon: AnimatedIcon(
        icon: AnimatedIcons.menu_arrow,
        progress: transitionAnimation,
      ),
      onPressed: () {
        close(context, '');
      },
    );
  }

  @override
  Widget buildResults(BuildContext context) {
    // TODO: implement buildResults
    return Center(
      child: Card(
        margin: EdgeInsets.all(30),
        child: Column(
          mainAxisSize: MainAxisSize.min,
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(Icons.mark_as_unread),
            Text(selected),
          ],
        ),
      ),
    );
  }

  @override
  Widget buildSuggestions(BuildContext context) {
    var suggestionList = words.where((s) => s.startsWith(query)).toList();
    return ListView.builder(
      itemBuilder: (context, index) => ListTile(
        leading: Icon(Icons.catching_pokemon),
        title: RichText(
          text: TextSpan(
              text: suggestionList[index].substring(0, query.length),
              style: TextStyle(
                  color: Colors.purple[300], fontWeight: FontWeight.bold),
              children: [
                TextSpan(
                  text: suggestionList[index]
                      .substring(query.length, suggestionList[index].length),
                  style: TextStyle(
                      color: Colors.black, fontWeight: FontWeight.normal),
                ),
              ]),
        ),
        onTap: () => query = selected = suggestionList[index],
      ),
      itemCount: suggestionList.length,
    );
  }
}
