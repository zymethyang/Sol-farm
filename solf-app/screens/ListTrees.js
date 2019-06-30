import React from 'react';
import {
  StyleSheet,
  Text,
  View,
  Image,
  TouchableOpacity
} from 'react-native';

const data = {
  rau_can: {
    name: 'Rau cần',
    phMin: 6,
    phMax: 8,
    ecMin: 2,
    ecMax: 3,
    image: require('../assets/images/vegetables/rau_can.jpg')
  },
  su_su: {
    name: 'Su Su',
    phMin: 6,
    phMax: 8,
    ecMin: 5,
    ecMax: 6,
    image: require('../assets/images/vegetables/su_su.jpg')
  }
}

export default class ListTrees extends React.Component {
  static navigationOptions = {
    title: 'List trees',
  }

  renderTree = (treesList) => {
    let result = null;
    result = treesList.map((tree, index) => {
      return (
        <TouchableOpacity style={styles.wrapSelection} key={index} onPress={() => this.props.navigation.navigate('AutoScreen', tree)}>
          <View style={styles.alignCenter}>
            <Image source={tree.image} style={styles.wrapImage} />
          </View>
          <View style={styles.wrapName}>
            <Text style={styles.nameStyle}>{tree.name}</Text>
          </View>
        </TouchableOpacity>
      );
    })
    return result;
  }

  render() {
    return (
      <View style={styles.container}>
        {this.renderTree(Object.values(data))}
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
  },
  wrapSelection: {
    height: 80,
    flexDirection: 'row',
    marginLeft: 20,
    marginRight: 20,
    marginTop: 10
  },
  wrapImage: {
    width: 60,
    height: 60,
    borderRadius: 30
  },
  nameStyle: {
    fontSize: 25,
    fontFamily: 'cereal-medium',
    color: '#00bfa5'
  },
  wrapName: {
    marginLeft: 20,
    alignSelf: 'center'
  },
  alignCenter: {
    alignSelf: 'center'
  }
});
