import Amplify from 'aws-amplify';
import { AWSIoTProvider } from '@aws-amplify/pubsub/lib/Providers';

Amplify.addPluggable(new AWSIoTProvider({
    aws_pubsub_region: 'ap-southeast-1',
    aws_pubsub_endpoint: 'wss://a2184o3gtkvd1o-ats.iot.ap-southeast-1.amazonaws.com/mqtt',
}));

Amplify.configure({
    Auth: {
        identityPoolId: 'ap-southeast-1:d09bc625-a624-4148-a445-9a2a50f0f7e6',
        region: 'ap-southeast-1',
    }
})

export default Amplify;