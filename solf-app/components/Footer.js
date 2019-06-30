import React from 'react';
import {
  StyleSheet,
  Image,
} from 'react-native';


export default class Footer extends React.Component {
  render() {
    return (
      <Image
        resizeMode={'cover'}
        style={styles.imgStyle}
        source={require('../assets/images/footer.jpg')}
      />
    );
  }
}

const styles = StyleSheet.create({
  imgStyle: {
    width: '100%',
    height: 200,
    position: 'absolute',
    bottom: 0,
    left: 0
  }
});
