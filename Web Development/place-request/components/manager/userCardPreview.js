'use client'

import {
    CardBody,
    Card,
    Flex,
    Box,
    Link,
    Heading,
    Text,
} from "@chakra-ui/react";
import UserIcon from "../shared/userIcon";

export default function UserCardPreview({ user }) {

    return (
        <Link href={`/manager/user?user_id=${user.id}`}
            style={{
                textDecoration: 'none'
            }} >
            <Card w='750px' h='auto'>
                <CardBody>
                    <Flex justify="flex-start" align='center' w='700px' >
                        <UserIcon />
                        <Box w="4" />
                        <Heading fontSize='lg' maxW='700px' noOfLines={[1]} >{user.username}</Heading>
                        <Box w="4" />
                        <Text color='grey' fontSize='md'>id: {user.id}</Text>
                    </Flex>
                </CardBody>
            </Card>
        </Link >
    );
}
