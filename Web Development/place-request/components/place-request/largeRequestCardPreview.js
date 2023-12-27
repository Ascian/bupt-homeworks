import {
    Card,
    CardBody,
    Image,
    Heading,
    Text,
    Flex,
    Box,
    Link,
} from "@chakra-ui/react";

export default async function LargeRequestCardPreview({ request }) {
    const date = new Date(request.createTime);
    const createTime = date.toLocaleDateString()

    return (
        <Link
            href={`/request?request_id=${request.seekerId}`}
            style={{
                textDecoration: 'none'
            }}>
            <Card w='300px' h='auto' overflow="hidden">
                <CardBody>
                    <Image width='full' src="/picture.png" alt="picture" />
                    <Heading size='lg' maxW='250px' noOfLines={[1]}>{request.seekerTitle}</Heading>
                    <Box h='2' />
                    <Text fontSize='sm' color='grey' >{`${createTime}    ${request.region}`}</Text>
                    <Box h='2' />
                    <Text fontSize='md' maxW='240px' maxH='140px' noOfLines={[1, 2, 3, 4]}>{request.seekerDescription}</Text>
                    <Box h='2' />
                    <Text fontSize='md' color='grey' >{request.destinationType}</Text>
                    <Box h='4' />
                    <Flex justify="flex-start" align='center' w='240px' >
                        <Flex justify="flex-start" align='center' w='240px' >
                            <Image borderRadius="full" boxSize="40px" src="/userIcon.png" alt="User Icon" />
                            <Box w="2" />
                            <Text fontSize='md' >{request.username}</Text>
                        </Flex>
                        <Flex justify="flex-end" align='center' w='240px' >
                            <Text fontSize='lg' >￥{request.maxExpectedPrice}</Text>
                        </Flex>
                    </Flex>
                </CardBody>
            </Card >
        </Link >
    );
}